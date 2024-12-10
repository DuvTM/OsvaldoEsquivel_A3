#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLRETURN ret;

    // Asignar un gestor de entorno
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

    // Asignar un gestor de conexión
    ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

    // Conectarse a la base de datos
    ret = SQLConnect(hDbc, (SQLWCHAR*)L"Empresa", SQL_NTS, (SQLWCHAR*)L"Username", SQL_NTS, (SQLWCHAR*)L"Password", SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {

        cout << "Conectado a la base de datos exitosamente." << endl;

        std::cout << std::endl;

        SQLHSTMT hStmt;
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
        std::cout << "Empleados" << std::endl;
        wcout << L"+-------+--------------------------+---------------------+---------------+------------------+---------------+-----------+ " << endl;
        wcout << L"| N.    | Nombre completo          | Fecha de nacimiento | RFC           | Nombre de centro | Puesto        | Directivo | " << endl;
        wcout << L"+-------+--------------------------+---------------------+---------------+------------------+---------------+-----------+ " << endl;

        //Select a la base de datos
        ret = SQLExecDirect(hStmt, (SQLWCHAR*)L"SELECT * FROM Empleados", SQL_NTS);

        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
            int num_empleado;
            SQLWCHAR name[50];
            SQLWCHAR last_name[50];
            SQLWCHAR slast_name[50];
            SQLWCHAR birthdate[50];
            SQLWCHAR RFC[50];
            SQLWCHAR center_name[50];
            SQLWCHAR position[50];
            SQLWCHAR executive[50];
            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &num_empleado, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_WCHAR, name, sizeof(name), NULL);
                SQLGetData(hStmt, 3, SQL_C_WCHAR, last_name, sizeof(last_name), NULL);
                SQLGetData(hStmt, 4, SQL_C_WCHAR, slast_name, sizeof(slast_name), NULL);
                SQLGetData(hStmt, 5, SQL_C_WCHAR, birthdate, sizeof(birthdate), NULL);
                SQLGetData(hStmt, 6, SQL_C_WCHAR, RFC, sizeof(RFC), NULL);
                SQLGetData(hStmt, 8, SQL_C_WCHAR, center_name, sizeof(center_name), NULL);
                SQLGetData(hStmt, 10, SQL_C_WCHAR, position, sizeof(position), NULL);
                SQLGetData(hStmt, 11, SQL_C_WCHAR, executive, sizeof(executive), NULL);

                wstring full_name = wstring(name) + L" " + wstring(last_name) + L" " + wstring(slast_name);
                wstring Directivo = wstring(executive);
                wstring respuesta;

                if (Directivo == L"1") {
                    respuesta = L"Si";
                }
                else
                {
                    (Directivo == L"0");
                    respuesta = L"No";
                }


                wcout << L"| " << setw(6) << left << num_empleado
                    << L"| " << setw(25) << left << full_name
                    << L"| " << setw(20) << left << birthdate
                    << L"| " << setw(14) << left << RFC
                    << L"| " << setw(17) << left << center_name
                    << L"| " << setw(14) << left << position
                    << L"| " << setw(9) << left << executive << L" |" << endl;

            }
            wcout << L"+-------+--------------------------+---------------------+---------------+------------------+---------------+-----------+ " << endl;

        }
        // Liberar el manejador de conexión
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    }

    else {
        cout << "Fallo la conexion a la base de datos" << endl;
    }

    // Desconectar y liberar gestores de entorno
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    return 0;
}