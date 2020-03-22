Quick Docker image for cryptseod
---------------------------

Build docker image:
   
    docker/build.sh

Push docker image:

    docker/push.sh

Pull cryptseocore/cryptseo:latest from docker hub  at [cryptseo-dockerhub](https://hub.docker.com/r/cryptseocore/cryptseo/)

    sudo docker pull cryptseocore/cryptseo
    
Run docker image

    sudo docker run cryptseocore/cryptseo

Build docker for cryptseod
----------
A Docker configuration with cryptseod node by default.

    sudo apt install apt-transport-https ca-certificates curl software-properties-common; curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -; sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"; sudo apt-get update; sudo apt install docker-ce   
---------------------------------------------------        
    
    mkdir cryptseo-mounted-data
    docker run --name cryptseo -d \
     --env 'CSEO_RPCUSER=rpciser' \
     --env 'CSEO_RPCPASSWORD=rpcpassword' \
     --env 'CSEO_TXINDEX=1' \
     --volume ~/cryptseo-mounted-data:~/.cryptseo \
     -p 6480:6480 \
     --publish 6480:6480 \
     cryptseocore/cryptseo
----------------------------------------------------
Logs

    docker logs -f cryptseo

----------------------------------------------------

## Configuration

Set your `cryptseo.conf` file can be placed in the `cryptseo-mounted data` dir.
Otherwise, a default `cryptseo.conf` file will be automatically generated based
on environment variables passed to the container:

| name | default |
| ---- | ------- |
| BTC_RPCUSER | rpcuser |
| BTC_RPCPASSWORD | rpcpassword |
| BTC_RPCPORT | 6480 |
| BTC_RPCALLOWIP | ::/0 |
| BTC_RPCCLIENTTIMEOUT | 30 |
| BTC_DISABLEWALLET | 1 |
| BTC_TXINDEX | 0 |
| BTC_TESTNET | 0 |
| BTC_DBCACHE | 0 |
| BTC_ZMQPUBHASHTX | tcp://0.0.0.0:28333 |
| BTC_ZMQPUBHASHBLOCK | tcp://0.0.0.0:28333 |
| BTC_ZMQPUBRAWTX | tcp://0.0.0.0:28333 |
| BTC_ZMQPUBRAWBLOCK | tcp://0.0.0.0:28333 |


## Daemonizing

If you're daemonizing is to use Docker's 
[Daemonizing](https://docs.docker.com/config/containers/start-containers-automatically/#use-a-restart-policy),
but if you're insistent on using systemd, you could do something like

```
$ cat /etc/systemd/system/cryptseod.service

# cryptseod.service #######################################################################
[Unit]
Description=Cryptseo
After=docker.service
Requires=docker.service

[Service]
ExecStartPre=-/usr/bin/docker kill cryptseo
ExecStartPre=-/usr/bin/docker rm cryptseo
ExecStartPre=/usr/bin/docker pull cryptseocore/cryptseo
ExecStart=/usr/bin/docker run \
    --name cryptseo \
    -p 6480:6480 \
    -p 6480:6480 \
    -v /data/cryptseod:/root/.cryptseo \
    cryptseocore/cryptseo
ExecStop=/usr/bin/docker stop cryptseo
```
