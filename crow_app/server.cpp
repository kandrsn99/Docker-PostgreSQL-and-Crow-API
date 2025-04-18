#include "crow.h"
#include "pqxx/pqxx"

int main() {
    crow::SimpleApp app;

    crow::Route(app, "/employees")
    ([]() {
        try {
            // Get your connection.
            pqxx::connection conn("dbname=mydatabase user=myuser password=mypassword host=running_bouncer port=6432");

            pqxx::work txn(conn);
            pqxx::result res = txn.exec("SELECT id, name FROM mytable");
            txn.commit();

            crow::json::wvalue json_res;
            for (const auto& row : res) {
                json_res[row["id"].as<int>()] = row["name"].as<std::string>();
            }

            return crow::response(json_res.dump());
        } catch (const std::exception& e) {
            return crow::response(500, std::string("Error: ") + e.what());
        }
    });
    // Force address and port
    app.bindaddr("0.0.0.0").port(8080).multithreaded().run();
}
