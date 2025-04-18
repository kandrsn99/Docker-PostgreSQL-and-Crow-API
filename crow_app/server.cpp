#include "crow.h"
#include "pqxx/pqxx"
// Parse the database into json formatting.
crow::json::wvalue build_json_result(const pqxx::result& res) {
        // Declare a vector for pushing to parse our database table.
        std::vector<crow::json::wvalue> temp_array;
        // Construct JSON
        for (const auto& row : res) {
                // Construct a crow::json::wvalue to parse our database results.
                crow::json::wvalue employee;
                employee["first_name"] = row["first_name"].as<std::string>();
                employee["last_name"] = row["last_name"].as<std::string>();
                employee["vehicle"] = row["vehicle"].as<std::string>();
                // Push itoms onto the vector using push back.
                temp_array.push_back(std::move(employee));
        }
        // Now construct a crow::json:wvalue from the vector.
        crow::json::wvalue json_result(temp_array);
        // Spit it out
        return(json_result);
}
// Main API proxy
int main() {
    crow::SimpleApp app;

    crow::Route(app, "/employees").methods("GET"_method)([](const crow::request &req) -> crow::response {
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
            return crow::response(500, std::string("Error: ") + e.what());
        }
    });
    // Force address and port
    app.bindaddr("0.0.0.0").port(8080).multithreaded().run();
}
