const int MAX_BUFFER = 1024;

struct file_data {
    string filename<>;
    int size;
    opaque data<MAX_BUFFER>;
};

program FILE_TRANSFER {
    version FILE_TRANSFER_VERS {
        file_data FILE_TRANSFER_PROC(string) = 1;
    } = 1;
};