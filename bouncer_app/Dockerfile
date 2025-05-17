# Pull latest image
FROM edoburu/pgbouncer
# Copy the configuration for bouncer
COPY pgbouncer.ini /etc/pgbouncer/pgbouncer.ini
# Don't forget the hash from the database
COPY userlist.txt /etc/pgbouncer/userlist.txt
# Expose the proper port. Please note that we supersede the firewall and open this port. 
# It is best practice to not open this port if you do not intend to update the database remotely.
EXPOSE 6432
# Start up the buoncer.
CMD ["pgbouncer", "/etc/pgbouncer/pgbouncer.ini"]
