class Codes:
    def __init__(self):
        # requests codes
        self.login_request = 1
        self.signup_request = 2

        # error codes
        self.unexpected_error = 0

        # login codes
        self.login_succeed = 1
        self.wrong_password = 2
        self.wrong_username = 3

        # signup codes
        self.signup_succeed = 4
        self.username_exists = 5

        #nodes data gathering
        self.unable_to_find_data = 10
        self.got_guard_node = 11
        self.found_node_location = 12
