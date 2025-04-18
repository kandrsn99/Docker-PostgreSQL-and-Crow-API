#include "crow.h"
#include "pqxx/pqxx"
// Parse the database into json formatting.
crow::json::wvalue build_json_result(const pqxx::result& res) {
        // Declare a vector for pushing to parse our database table.
        std::vector<crow::json::wvalue> temp_array;
        // Construct JSON
        for (const auto& row : res) {
                // Construct a crow::json::wvalue to parse our database results.
                crow::json::wvalue employees;
                employees["first_name"] = row["first_name"].as<std::string>();
                employees["last_name"] = row["last_name"].as<std::string>();
                employees["vehicle"] = row["vehicle"].as<std::string>();
                // Push itoms onto the vector using push back.
                temp_array.push_back(std::move(employees));
        }
        // Now construct a crow::json:wvalue from the vector.
        crow::json::wvalue json_result(temp_array);
        // Spit it out
        return(json_result);
}
// Main API proxy
int main() {
        crow::SimpleApp app;
        // Post a new employee
        CROW_ROUTE(app, "/hire").methods("POST"_method)([](const crow::request &req) -> crow::response {
                // Test method called by user.
                if(req.method != crow::HTTPMethod::Post) {
                    // Wrong method sent.
                    return crow::response(405);
                }
                // Next steps
                try {
                    // Open connection with pgbouncer
                    pqxx::connection conn("dbname=mydatabase user=myuser password=mypassword host=running_bouncer port=6432");
                    pqxx::work txn(conn);
                    // Initialize the results.
                    pqxx::result res;
                    // Check for the parameters by initializing a pointer for the url sent.
                    const char* first_parameter = req.url_params.get("first");
                    const char* last_parameter = req.url_params.get("last");
                    const char* vehicle_parameter = req.url_params.get("vehicle");
                    // Check the parameters every time we are called up.
                    if(first_parameter && last_parameter && vehicle_parameter) {
                                // Parse our search value name parameter.
                                std::string first_pattern = "%" + std::string(first_parameter) + "%";
                                std::string last_pattern = "%" + std::string(last_parameter) + "%";
                                std::string vehicle_pattern = "%" + std::string(vehicle_parameter) + "%";
                                // Prepare SQL call.
                                res = txn.exec_params("INSERT INTO employees"
                                        " (first_name, last_name, vehicle)"
                                        " VALUES first_name ILIKE $1, last_name ILIKE $2, vehicle ILIKE $3;",
                                        first_pattern,
                                        last_pattern,
                                        vehicle_pattern
                                        );
                            INSERT INTO employees (first_name, last_name, vehicle) VALUES ($1, $2, $3);
                            
                    } else {
                            return crow::reponse(400, std::string("Check input!"));
                    }
                    // Transact
                    txn.commit();
                    // Dump JSON out for endpoint.
                    return crow::response(201, std::string("Created!"));
                } catch (const std::exception& e) {
                    // May check for more issues during development by adding + e.what()
                    return crow::response(500, std::string("Error!"));
                }
        });
        // Put an update on an employee
        CROW_ROUTE(app, "/update").methods("PUT"_method)([](const crow::request &req) -> crow::response {
                // Test method called by user.
                if(req.method != crow::HTTPMethod::Put) {
                    // Wrong method sent.
                    return crow::response(405);
                }
                // Next steps
                try {
                    // Open connection with pgbouncer
                    pqxx::connection conn("dbname=mydatabase user=myuser password=mypassword host=running_bouncer port=6432");
                    pqxx::work txn(conn);
                    // Initialize the results.
                    pqxx::result res;
                    // Check for the parameters by initializing a pointer for the url sent.
                    const char* first_parameter = req.url_params.get("first");
                    const char* last_parameter = req.url_params.get("last");
                    const char* vehicle_parameter = req.url_params.get("vehicle");
                    // Check the parameters every time we are called up.
                    if(first_parameter && last_parameter && vehicle_parameter) {
                                // Parse our search value name parameter.
                                std::string first_pattern = "%" + std::string(first_parameter) + "%";
                                std::string last_pattern = "%" + std::string(last_parameter) + "%";
                                std::string vehicle_pattern = "%" + std::string(vehicle_parameter) + "%";
                                // Prepare SQL call.
                                res = txn.exec_params("UPDATE employees"
                                        " SET vehicle ILIKE $1"
                                        " WHERE first_name ILIKE $2 AND last_name ILIKE $3;",
                                        vehicle_pattern,
                                        first_pattern,
                                        last_pattern
                                        );
                    } else {
                            return crow::reponse(400, std::string("Check input!"));
                    }
                    // Transact
                    txn.commit();
                    // Dump JSON out for endpoint.
                    return crow::response(200, std::string("Updated!"));
                } catch (const std::exception& e) {
                    // May check for more issues during development by adding + e.what()
                    return crow::response(500, std::string("Error!"));
                }
        });
        // Delete an employee
        CROW_ROUTE(app, "/fire").methods("DELETE"_method)([](const crow::request &req) -> crow::response {
                // Test method called by user.
                if(req.method != crow::HTTPMethod::Delete) {
                    // Wrong method sent.
                    return crow::response(405);
                }
                // Next steps
                try {
                    // Open connection with pgbouncer
                    pqxx::connection conn("dbname=mydatabase user=myuser password=mypassword host=running_bouncer port=6432");
                    pqxx::work txn(conn);
                    // Initialize the results.
                    pqxx::result res;
                    // Check for the parameters by initializing a pointer for the url sent.
                    const char* first_parameter = req.url_params.get("first");
                    const char* last_parameter = req.url_params.get("last");
                    // Check the parameters every time we are called up.
                    if(first_parameter && last_parameter) {
                                // Parse our search value name parameter.
                                std::string first_pattern = "%" + std::string(first_parameter) + "%";
                                std::string last_pattern = "%" + std::string(last_parameter) + "%";
                                // Prepare SQL call.
                                res = txn.exec_params("DELETE FROM employees"
                                        " WHERE first_name ILIKE $1 AND last_name ILIKE $2;",
                                        first_pattern,
                                        last_pattern
                                        );
                    } else {
                            return crow::reponse(400, std::string("Check input!"));
                    }
                    // Transact
                    txn.commit();
                    // Dump JSON out for endpoint.
                    return crow::response(200, std::string("Deleted!"));
                } catch (const std::exception& e) {
                    // May check for more issues during development by adding + e.what()
                    return crow::response(500, std::string("Error!"));
                }
        });
        // Get all employees
        CROW_ROUTE(app, "/employees").methods("GET"_method)([](const crow::request &req) -> crow::response {
                // Test method called by user.
                if(req.method != crow::HTTPMethod::Get) {
                    // Wrong method sent.
                    return crow::response(405);
                }
                // Next steps
                try {
                    // Open connection with pgbouncer
                    pqxx::connection conn("dbname=mydatabase user=myuser password=mypassword host=running_bouncer port=6432");
                    pqxx::work txn(conn);
                    // Create our SQL query
                    pqxx::result res = txn.exec("SELECT * FROM employees ORDER BY employee_id;");
                    // Transact
                    txn.commit();
                    // Create JSON
                    crow::json::wvalue build_json_result = build_system_json(res);
                    // Dump JSON out for endpoint.
                    return crow::response(json_res.dump());
                } catch (const std::exception& e) {
                    // May check for more issues during development by adding + e.what()
                    return crow::response(500, std::string("Error!"));
                }
        });
        // Force address and port
        app.bindaddr("0.0.0.0").port(8080).multithreaded().run();
}
