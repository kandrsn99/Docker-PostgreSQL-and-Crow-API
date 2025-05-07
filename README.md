# Docker-PostgreSQL-and-Crow

This is a sample of a PostgreSQL database that is launchable with Docker. Docker makes it easy to deploy executables on a server host with a web framework ontop of our PostgreSQL database, such as those with Crow Application Programmer Interface (API) Framework in C++ demonstrating a RESTful architecture for an API with NGINX for load balancing and serving a static documentation page. You may read more about Crow, https://crowcpp.org/master/, for documentation or benchmarking statistics.

You may read about the summarized documentation of Docker here: https://github.com/kandrsn99/Docker-PostgreSQL-and-Crow-API/blob/main/Docker%20Command%20Line%20Interface.pdf

It shall be noted here you will need to install the Docker Engine on your local machine. You may do this through the instructions on the following webpage: https://docs.docker.com/engine/install/

In order to install this repository from the command line you will need to get the 'git' package on your linux machine.

Any Operating System utilizing 'APT' package manager
> sudo apt-get git

Any Operating System utilizing 'YUM' package manager
> sudo yum install git

Any Operating System utilizing 'DNF' package manager
> sudo dnf install git

Next, utilize the git function on your local machine command line interface to download this repository.
> git clone https://github.com/kandrsn99/Docker-PostgreSQL-and-Crow-API.git

Subsequently, change into the directory containing your downloaded repository. 
> cd 'name of file-path'

You will need to change some variables prior to composing. Check your docker-compose.yml file and those labelled 'environment' which need to be declared for your database. You will do this with your operating systems text editor. It is typically 'nano' or 'vim' depending on the machines operating system.

Upon entering the directory, the docker compose file should allow you to build the database.
> docker compose build postgresql_proxy\
> docker compose up postgresql_proxy -d

## NOTE: Any reference to docker-compose.yml environment variables that you name is required for the commands. You will substitute those values manually that you initialized.
## NOTE: Any time you docker compose build you are recreating the same database that was supposed to be initialized on start up.
## NOTE: NGINX and PGBOUNCER may need to be restarted if anything is updated. You might see a database connection error or unable to reach your API depending on configuration or initial start up of containers.

Now, we may mosey into the running container and run commands in the PostgreSQL environment.
> docker exec -it running_postgresql bash\
> psql -U POSTGRES_USER (environment variable reference) POSTGRES_DB (environment variable reference)

You must retrieve the hashed password once inside the container for your database if you choose to use pgbouncer for pooling your connections instead of directly connecting to the postgresql database. A modern instance of postgresql will use scram-sha-256 for hashing and we will need it for pgbouncer.ini through the userlist.txt file. You will find this in the bouncer_app directory. Subsequently, inside your postgresql container you will run the following commands.

> psql -U your_user_name your_db_name \
> SELECT usename, passwd FROM pg_shadow;

You will copy and paste that to your userlist.txt file for the pgbouncer.ini for booting up your connection pool.

Please note that you must enter the running container if you wish to change the port with which the PostgreSQL environment will run.
> docker exec -it running_postgresql bash\
> cd /var/lib/postgresql/data

At which point you will open the postgres.conf file located in that directory with the text editor in that system. You may learn more about postgres.conf here at https://www.postgresql.org/docs/14/runtime-config-connection.html#RUNTIME-CONFIG-CONNECTION-SETTINGS
> #port = 5432\
> port = NUMBER (your favorite port)

And of course, change the docker-compose.yml and run a restart.
> docker compose down\
> docker compose up

Before we start the Crow API, we need the host name or internet protocol address of our PostgreSQL environment just in case the container name doesn't supplement the internal network address on startup.

> docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' running_postgresql

Now, that we have the address we shall use it to connect the Crow API to the database or through pgbouncer by modifying the script in /crow_app/ and using 'nano' or any other text editor to change the appropriately labeled variables when needed. Remember to add your secure socket layer certificates and edit the nginx.conf file in the appropriate directories before spooling up the rest of your containers.

Do note that you must retrieve an SSL (secure socket layer) certificate to have HTTPs working for your domain name. The NGINX configuration file is meant to be easy to follow and understand. Read it and make sure the certificates are stored in the correct locations with the proper naming schema for NGINX. An easy way to create SSL certificate may be done with openSSL, https://openssl.org/, from the command line or otherwise downloaded from the DNS provider. 

It is highly recommended that you use Cloudflare as they are the leading provider of a register for hosting a DNS. You may review their documentation here https://developers.cloudflare.com/learning-paths/get-started/ at your leisure.

And of course, we spool both containers up for crow proxies, pgbouncer, and nginx proxy
> docker compose build crow_proxy \
> docker compose up crow_proxy -d \
> docker compose build pg_bouncer \
> docker compose up pg_bouncer - d \
> docker compose build nginx_proxy\
> docker compose up nginx_proxy -d

You may test your API on https://your_domain.com/api/documentation or http://address/api/documentation at your discretion.

You may read about the official PostgreSQL docker image documentation here https://hub.docker.com/_/postgres/ and the official rocker image https://rocker-project.org/images/versioned/rstudio.html which was modified for this repository.
