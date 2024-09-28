#include <iostream>
#include <libpq-fe.h>

void checkConnStatus(PGconn *conn) {
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection to database failed: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        exit(1);
    }
}

int main() {
    // Conectar a la base de datos
    PGconn *conn = PQconnectdb("dbname=compania_x user=tu_usuario password=tu_contraseña");
    checkConnStatus(conn);

    // Crear la tabla usuarios
    const char *createTableQuery = "CREATE TABLE IF NOT EXISTS usuarios ("
                                   "id SERIAL PRIMARY KEY, "
                                   "nombre VARCHAR(50), "
                                   "apellido VARCHAR(50), "
                                   "email VARCHAR(100), "
                                   "fecha_registro DATE);";
    PGresult *res = PQexec(conn, createTableQuery);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Create table failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }
    PQclear(res);

    // Insertar datos ficticios
    const char *insertQuery = "INSERT INTO usuarios (nombre, apellido, email, fecha_registro) VALUES "
                              "('Juan', 'Pérez', 'juan.perez@example.com', '2023-01-15'),"
                              "('María', 'Gómez', 'maria.gomez@example.com', '2023-02-20'),"
                              "('Carlos', 'López', 'carlos.lopez@example.com', '2023-03-10'),"
                              "('Ana', 'Martínez', 'ana.martinez@example.com', '2023-04-05'),"
                              "('Luis', 'Fernández', 'luis.fernandez@example.com', '2023-05-25');";
    res = PQexec(conn, insertQuery);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Insert data failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }
    PQclear(res);

    // Verificar los datos insertados
    res = PQexec(conn, "SELECT * FROM usuarios;");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Select data failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }

    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        std::cout << "ID: " << PQgetvalue(res, i, 0) << ", "
                  << "Nombre: " << PQgetvalue(res, i, 1) << ", "
                  << "Apellido: " << PQgetvalue(res, i, 2) << ", "
                  << "Email: " << PQgetvalue(res, i, 3) << ", "
                  << "Fecha de Registro: " << PQgetvalue(res, i, 4) << std::endl;
    }

    PQclear(res);
    PQfinish(conn);

    return 0;
}
