require "socket"

def d x
  if $debug
    printf("%f:	", Time.now - $last_tm)
    p x
  end
end

def d2 x
  if $debug
    printf("%f:	", Time.now - $last_tm)
    p x
  end
end

class WritePort
  def initialize(nm, s)
    @nm = nm
    @s = s
  end
  def send_treq(task_head, treq_head)
    d2 [@nm, "TREQ", task_head, treq_head]
    @s.printf("treq %s %s\n", task_head, treq_head)
    @s.flush
  end
  def send_task(ndiv, rslt_head, task_head, body)
    d2 [@nm, "TASK", ndiv, rslt_head, task_head]
    @s.printf("task %d %s %s\n", ndiv, rslt_head, task_head)
    @s.flush
    @s.print body
    @s.flush
  end
  def send_none(task_head)
    d2 [@nm, "NONE", task_head]
    @s.printf("none %s\n", task_head)
    @s.flush
  end
  def send_rslt(rslt_head, rbody)
    d2 [@nm, "RSLT", rslt_head]
    @s.printf("rslt %s\n", rslt_head)
    @s.flush
    @s.print rbody
    @s.flush
  end
  def send_rack(task_head)
    d2 [@nm, "RACK", task_head]
    @s.printf("rack %s\n", task_head)
    @s.flush
  end
  def send_exit
    d2 [@nm, "EXIT"]
    @s.printf "exit\n"
    @s.flush
  end
end

class AbsChild
  def initialize
    @divisible = false
    @in_treq = false 
    @abs_ws = []
    @stock = []
  end
  def in_treq?
    return @in_treq
  end
  def empty?
    return (@abs_ws.length == 0)
  end
  def divisible?
    @divisible
  end
  def divisible
    @divisible = true
  end
  def indivisible
    @divisible = false
  end
  def ndiv
    @abs_ws.last
  end
  def pre_div
    unless empty?
      @abs_ws[-1] += 1
    end
  end
  def cancel_pre_div
    unless empty?
      @abs_ws[-1] -= 1
    end
  end
  def push_task(ndiv)
    @abs_ws << ndiv
    @in_treq = false
  end
  def get_none
    @in_treq = false
  end
  def update_task(ndiv)
    @abs_ws[-1] = ndiv
  end
  def pop_task()
    @abs_ws.pop
  end
  def req_task
    @in_treq = true
  end
end

class ChildWritePort < WritePort
  def initialize(nm, s, ac)
    super(nm, s)
    @ac = ac
  end
  def send_treq(task_head, treq_head)
    if !@ac.divisible?
      return nil
    end
    @ac.pre_div
    super
  end
  def send_task(ndiv, rslt_head, task_head, tbody)
    super
    @ac.push_task(ndiv) # should be after "super"
  end
  def send_none(task_head)
    super
    @ac.get_none # should be after "super"
  end
  def ac
    @ac
  end
end

class FCWritePort < ChildWritePort
end

class SCWritePort < ChildWritePort
end

class PRWritePort < WritePort
end

##

class TreqAnyTable
  def initialize
    @hs = []    # header (treq from ...)
    @acs = []   # AbsChild
    @cwps = []  # Child Write Ports
    @routs = []
    @dvbls = []
  end
  def inspect
    @hs.inspect
  end
  def add_child(ac, cwp, rout)
    @acs << ac
    @cwps << cwp
    @routs << rout
    # @cwps[0]がfcに対応し、@cwps[1]がscs[0]に対応する
    # ちょっとわかりにくい...
  end
  def treq_include?(th)
    @hs.include?(th)
  end
  def reg_treq(th)
    @hs << th if !treq_include?(th)
  end
  def del_treq(th)
    @hs.delete(th)
    d ["del_treq", th, @hs]
  end
  def dvbl_include?(wp)
    @dvbls.include?(wp)
  end
  def reg_dvbl(wp)
    @dvbls << wp if !dvbl_include?(wp)
    wp.ac.divisible
  end
  def del_dvbl(wp)
    @dvbls.delete(wp)
    wp.ac.indivisible
  end
  def biggest_dvbl
    bdv, min = nil, nil
    for cwp in @dvbls
      ac = cwp.ac
      ndiv = ac.ndiv
      if (min.nil? || ndiv < min)
        bdv, min = cwp, ndiv
      end
    end
    return bdv
  end
  def retry_treq
    i = 0
    d ["retry_treq"]
    @hs.each{|th|
      from = th.to_i+1
      ac = @acs[from]
      tg = biggest_dvbl
      if !tg.nil? && !ac.in_treq?
        ac.req_task
        @routs[from].try_send_treq(tg, th, "any")
        i += 1
      end
      break if i == $n_retry
    }
  end
end

class Router
  @@tat = TreqAnyTable.new
  def initialize(from, pr, fc, scs)
    # from = "p", "f", 0, 1, 2,...
    @from = from
    @rd = (from == "f" ? pr : fc)
    @hd_from = from.to_s + ':'
    @pr = pr
    @fc = fc
    @scs = scs
    @wp_from = case @from
               when "p"
                 @pr
               when "f"
                 @fc
               else
                 @scs[@from]
               end
  end
  def head_add(h)
    @hd_from + h
  end
  def head_shift(h)
    a = h.split(':')
    r = a.shift
    t = case r
        when "p"
          @pr
        when "f"
          @fc
        else
          @scs[r.to_i]
        end
    n = a.join(':')
    if defined? yield
      return yield(t, n)
    end
    [t, n]
  end
  def refuse_treq(n_task_head, tgt)
    head_shift(n_task_head){|tg, nh| tg.send_none(nh)}
  end
  def try_send_treq(tgt, n_task_head, n_treq_head)
    tgt.send_treq(n_task_head, n_treq_head) || refuse_treq(n_task_head, tgt)
  end
  # should be overridden
  def try_send_treq_any(n_task_head, n_treq_head) # n_treq_head == "any"
    try_send_treq(@rd, n_task_head, n_treq_head)
  end
  def proc_treq a
    task_head = a[1]
    treq_head = a[2]  # any or not
    n_task_head = head_add(task_head)
    if treq_head == "any"
      try_send_treq_any(n_task_head, treq_head)
    else  # treq_head != "any"
      head_shift(treq_head){|tg, nh| try_send_treq(tg, n_task_head, nh)}
    end
  end
  def proc_task(a,b)
    ndiv = a[1].to_i
    rslt_head = a[2]
    task_head = a[3]
    n_rslt_head = head_add(rslt_head)
    head_shift(task_head){|tg, nh|
      tg.send_task(ndiv, n_rslt_head, nh, b)
      @@tat.del_treq(task_head)
      # @@tat.retry_treq(tg)
    }
  end
  def proc_none a
    task_head = a[1]
    head_shift(task_head){|tg, nh|
      if !@@tat.treq_include?(task_head)
        tg.send_none(nh)
      else
        tg.ac.get_none
        @@tat.del_dvbl(@wp_from)
        @@tat.retry_treq
      end
    }
  end
  def proc_rslt(a,b)
    rslt_head = a[1]
    head_shift(rslt_head){|tg, nh| tg.send_rslt(nh, b)}
  end
  def proc_rack a
    task_head = a[1]
    head_shift(task_head){|tg, nh| tg.send_rack(nh) }
  end
  def proc_exit
    @wp_from.send_exit
  end
end

class ChildRouter < Router
  def initialize(from, pr, fc, scs, ac)
    super(from, pr, fc, scs)
    @ac = ac
  end
  def proc_treq a
    @ac.req_task
    @@tat.del_dvbl(@wp_from)
    super
  end
  def proc_task(a,b)
    @ac.update_task(a[1].to_i)
    super
  end
  def proc_none a
    @ac.cancel_pre_div
    @@tat.del_dvbl(@wp_from)
    super
  end
  def try_send_treq(tgt, n_task_head, n_treq_head)
    super
  end
  def refuse_treq(n_task_head, tgt)
    if !@@tat.treq_include?(n_task_head)
      super
    else
      head_shift(n_task_head){|tg, nh|
        tg.ac.get_none
        @@tat.del_dvbl(@wp_from)
        @@tat.retry_treq
      }
    end
  end
  def proc_rslt(a,b)
    @ac.pop_task
    @@tat.del_dvbl(@wp_from)
    super
  end
  def acinfo
    @ac
  end
  def proc_dvbl
    @@tat.reg_dvbl(@wp_from)
    @@tat.retry_treq
  end
end

class FCRouter < ChildRouter
  def initialize(from, pr, fc, scs, ac)
    super
    @@tat.add_child(ac, fc, self)
  end
end

class SCRouter < ChildRouter
  def initialize(from, pr, fc, scs, ac)
    super
    @last_i = 0
    @last_j = $num_any
    @@tat.add_child(ac, scs.last, self)
  end
  def find_biggest_sc
    bsc, min = nil, nil
    for sc in @scs
      ac = sc.ac
      ndiv = ac.ndiv
      if ac.divisible? && (min.nil? || ndiv < min) && ac != @ac
        bsc, min = sc, ndiv
      end
    end
    return bsc
  end
  def select_sc
    @last_j -= 1
    if @last_j <= 0
      @last_j = $num_any
      i = @last_i
      loop do
        i -= 1
        i += @scs.length if i < 0 
        sc = @scs[i]
        ac = sc.ac
        if ac.divisible? && ac != @ac
          @last_i = i 
          return sc
        end
        return nil if i == @last_i
      end
    end
    # find_biggest_sc
    @@tat.biggest_dvbl
  end
  def try_send_treq_any(n_task_head, n_treq_head) # n_treq_head == "any"
    ac = (@from == 0 ? @fc : @scs[(@from-1)/2]).ac
    if $const_any
      sc = (@from == 0 ? @fc : @scs[(@from-1)/2])
    else
      # sc = select_sc
      sc = @@tat.biggest_dvbl
    end
    @@tat.reg_treq(n_task_head)
    if !sc.nil? && sc.ac.divisible?
      try_send_treq(sc, n_task_head, n_treq_head)
    else
      refuse_treq(n_task_head, sc)
    end
  end
end

class PRRouter < Router
end

##

class ReadPort
  def initialize(s, rout, nm)
    @s = s
    @rout = rout
    @nm = nm
  end
  def close
    @rout.proc_exit
    @s.close
  end
  def proc_cmd
    cmd = @s.gets
    exit 0 if cmd.nil?
    a = cmd.split
    d ["read", @nm, a]
    case a[0]
    when "treq"
      @rout.proc_treq a
    when "task"
      # b = @s.gets
      b, li = [], nil
      while (li = @s.gets)[-2, 2] == "(\n"
        b << li
        li = @s.gets
        b << li
        c = li.split
        size = c[0].to_i
        b << @s.read(size)
      end
      b << li
      @rout.proc_task(a,b)
    when "none"
      @rout.proc_none a
    when "rslt"
      # b = @s.gets
      b, li = [], nil
      while (li = @s.gets)[-2, 2] == "(\n"
        b << li
        li = @s.gets
        b << li
        c = li.split
        size = c[0].to_i
        b << @s.read(size)
      end
      b << li
      @rout.proc_rslt(a,b)
    when "rack"
      @rout.proc_rack a
    when "staa"
      $scs.each{|s| p s.acinfo }
    when "stat"
      d $pr
      d $fc
      d $scs
    when "dvbl"
      @rout.proc_dvbl
    else
      d ["hen", a]
    end
  end
  def acinfo
    @rout.acinfo
  end
end

##

begin
  pr = "localhost:8881" # 親拠点サーバアドレス:ポート
  port_f = 8882 # 嫡子ポート
  port_s = 8883 # 庶子ポート
  $num_any = 10
  $n_retry = 20
  while a = ARGV.shift
    case a
    when "-p"
      pr = ARGV.shift
    when "-f"
      port_f = ARGV.shift.to_i
    when "-s"
      port_s = ARGV.shift.to_i
    when "-a"
      $num_any = ARGV.shift.to_i
    when "-d"
      $debug = true
    when "-c"
      $const_any = true
    when "-n"
      $n_retry = ARGV.shift.to_i
    else
      ARGV.unshift a
      break
    end
  end
  a = pr.split(':')
  if a[1].to_i == 0 || port_f == 0 || port_s == 0 || port_f == port_s
    puts "sp2.rb [-p host:port] [-f port] [-s port] [-a num_any] [-d] [-c]"
    exit 1
  end

  s_fc0 = TCPserver.open(port_f)
  s_scs0 = TCPserver.open(port_s)

  s_fc = s_fc0.accept
  s_fc0.close
  print("The first ", s_fc, " is accepted.\n")

  s_pr = nil

  host = a[0]
  port = a[1].to_i
  begin
    s_pr = TCPSocket.open(host, port)
  rescue
    retry
  end
  print(s_pr, " is connected.\n")

  ac_fc = AbsChild.new

  prwp = PRWritePort.new('p', s_pr)
  fcwp = FCWritePort.new('f', s_fc, ac_fc)
  scwps = []

  $scs = []
  $pr = ReadPort.new(s_pr, PRRouter.new('p', prwp, fcwp, scwps), 'p')
  fcrout = FCRouter.new('f', prwp, fcwp, scwps, ac_fc)
  $fc = ReadPort.new(s_fc, fcrout, 'f')

  # scwps.each{|wp| wp.send_none ["."]}
  # scrts.each_with_index{|rt,i| rt.proc_treq ["treq", i.to_s + ":.", "any"] }

  # d $pr, $fc, $scs

  s_scs = []
  socks = [s_pr, s_fc, s_scs0]
  loop do
    n = select(socks)
    next if n == nil
    $last_tm = Time.now unless $last_tm
    for s in n[0]
      case s
      when s_pr
        $pr.proc_cmd
      when s_fc
        $fc.proc_cmd
      when s_scs0
        s_sc = s_scs0.accept
        print("A secondary ", s_sc, " is accepted.\n")
        s_scs << s_sc
        socks << s_sc
        i = $scs.length
        ac = AbsChild.new
        scwp = SCWritePort.new(i, s_sc, ac)
        scwps << scwp
        scrout = SCRouter.new(i, prwp, fcwp, scwps, ac)
        $scs << ReadPort.new(s_sc, scrout, i)
      else
        $scs[s_scs.index(s)].proc_cmd
      end
    end
  end
ensure
  s_scs0.close
  $pr.close
  $fc.close
  $scs.each{|s| s.close }
  d "end..."
end
