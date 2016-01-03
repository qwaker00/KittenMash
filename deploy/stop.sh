echo 'Stopping'

sudo ls >/dev/null

sudo kill -SIGTERM $(<daemon.pid)
echo "Stop deamon: pid $(<daemon.pid)"

kill -SIGTERM $(<updater.pid)
echo "Stop updater: pid $(<updater.pid)"

echo 'Done'
