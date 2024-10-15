#ifndef UTIL_ERROR_H
#define UTIL_ERROR_H

#include <exception>

/*
 * Clase para encapsular el `errno` de C, "el ultimó error".
 * Es una excepción genérica que se encarga de los detalles
 * de decodificar el `errno` en un mensaje más entendible.
 * */
class UtilError: public std::exception {
    // cppcheck-suppress unusedStructMember
    char msg_error[256];  // Si se usa, falso positivo.

public:
    /*
     * Dado que `errno` es una variable global y puede ser modificada por
     * cualquier función, hay que hacer una copia apenas se haya detectado
     * el error, para luego lanzar `UtilError` o bien lanzar `UtilError` apenas
     * ya se haya detectado el error.
     *
     * El constructor `UtilError` es variadico y recibe, ademas del `errno`,
     * un format-string (como `printf`) y cero o más argumentos que
     * formaran parte del mensaje.
     *
     * int ret = foo();
     * if (ret == -1)
     *      throw UtilError(errno, "The function %s has failed: ", "foo");
     *  */
    UtilError(int error_code, const char* fmt, ...) noexcept;

    virtual const char* what() const noexcept;

    virtual ~UtilError();
};

#endif
