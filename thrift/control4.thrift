namespace cpp c4

service DriverService
{
    string doAction(1: i32 driverId);
}

service DirectorService
{
    string getValue(1: i32 id);
}
