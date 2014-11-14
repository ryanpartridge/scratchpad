namespace cpp sample

struct Message
{
    1: i32 key
    2: string value
}

service MessageService
{
    Message getMessage(1: i32 key)
    void setMessage(1: Message message)
}
