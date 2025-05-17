# Start the build process for an initial table.
FROM postgres:latest
# Add files properly
ADD ./schema.sql /docker-entrypoint-initdb.d
ADD ./vehiclestable.csv /docker-entrypoint-initdb.d
# Make sure proper port is open for PostgreSQL environment. Please note that we supersede the firewall and open this port. 
# It is best practice to not open this port if you do not intend to update the database remotely.
EXPOSE 5432
# Run the sql script to create a table
RUN chmod a+r /docker-entrypoint-initdb.d/*
