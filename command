gcc -o transjs main.c && cat sampleInput.json | ./transjs
gcc -o sysjoiner sysjoiner.c && cat sampleSyslogSplit.json | ./sysjoiner