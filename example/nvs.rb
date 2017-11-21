# initialize nvs: nvs_flash_init()
ESP32::NVS.init

# Get a Handle: nvs_handle;
nvs = ESP32::NVS.new "test-nvs", ESP32::NVS::READ_WRITE

begin 
  val=nvs["foo"]
  puts "Persisted 'foo': #{val}"
rescue
  # ***
  # Value should persist on next reboot
  # 
  # ***

  nvs["foo"] = 1979
  puts "Stored 'foo': #{nvs["foo"]}"
end

begin 
  val=nvs.get_string "hello"
  puts "Persisted 'hello': #{val}"
rescue
  # ***
  # Value should persist on next reboot
  # 
  # ***

  nvs["hello"] = "world"
  puts "Stored 'hello': #{nvs.get_string("hello")}"
end

# Loop forever

while true
  ESP32::System.delay(1000)
end
