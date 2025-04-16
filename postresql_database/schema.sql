/*
    Create a table if it doesn't exist on start
*/
CREATE TABLE IF NOT EXISTS employees
(
    id SERIAL PRIMARY KEY,
    first_name VARCHAR(50) NOT NULL,
    last_name VARCHAR(50) NOT NULL,
    vehicle VARCHAR(50) NOT NULL
);
/*
    Insert some values into that database table
*/
COPY employees (first_name, last_name, vehicle) 
FROM '/docker-entrypoint-initdb.d/vehiclestable.csv'
DELIMITER ','
CSV HEADER;
