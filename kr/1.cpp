class IOWrapper {
private:
    handle_t handle;
public:
    IOWrapper() = delete;
    IOWrapper& operator=(IOWrapper&) = delete;
    IOWrapper(IOWrapper&) = delete;
    IOWrapper(handle_t& curr_handle)
    {
        handle = curr_handle;
    }
    IOWrapper(handle_t&& curr_handle)
    {
        handle = std::move(curr_handle);
    }
    IOWrapper& operator=(IOWrapper&& x) 
    {
        handle = x.handle;
        x.handle = kNullHandle;
    }
    IOWrapper(IOWrapper&& x): handle(x.handle) 
    {
        x.handle = kNullHandle;
    }
    void Write(const std::string& content) 
    {
        if (handle != kNullHandle)
        raw_write(handle, content);
    }
    ~IOWrapper() 
    {
        if (handle != kNullHandle)
        raw_close(handle);
    }
};