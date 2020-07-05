from xmlrpc.server import SimpleXMLRPCServer

from primerandom import get_random_array, get_random_number


def main():
    server = SimpleXMLRPCServer(("127.0.0.1", 8080))
    server.register_function(get_random_number)
    server.register_function(get_random_array)

    server.serve_forever()


if __name__ == '__main__':
    main()
