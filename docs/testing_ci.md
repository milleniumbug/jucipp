# How to test Continuous Integration containers locally

### Requirements
 * [docker-engine](https://docs.docker.com/engine/installation) v1.10 or newer
 * [docker-compose](https://github.com/docker/compose/releases) v1.7.1 


```bash
git clone https://github.com/cppit/jucipp.git --recursive
cd jucipp/ci
docker-compose up <distro> # Note: Some systems might need to run as a privileged user
```