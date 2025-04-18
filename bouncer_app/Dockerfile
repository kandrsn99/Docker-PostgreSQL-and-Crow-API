# Pull latest image
FROM edoburu/pgbouncer
# Copy the configuration for bouncer
COPY pgbouncer.ini /etc/pgbouncer/pgbouncer.ini
# Don't forget the hash from the database
COPY userlist.txt /etc/pgbouncer/userlist.txt
# Expose the proper port.
EXPOSE 6432
# Start up the buoncer.
CMD ["pgbouncer", "/etc/pgbouncer/pgbouncer.ini"]
