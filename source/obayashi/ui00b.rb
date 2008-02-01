require "socket"

port = (ARGV.shift || 8881).to_i
gs = TCPserver.open(port)
addr = gs.addr
addr.shift
printf("UI server is on %s\n", addr.join(":"))
a = gs.accept
gs.close
$stdout.flush

$last_tm = Time.now

Thread.start do
  print(a, "is accepted.\n")
  while a.gets
    printf("%f:	", Time.now - $last_tm)
    print
    $stdout.flush
  end
end

while gets
  $last_tm = Time.now
  a.write($_)
end
a.close
