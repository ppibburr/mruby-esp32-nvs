module ESP32
  class NVS
    def initialize namespace, mode
      @data = self.class._open(namespace, mode)
    end
    
    def set_numeric key, value, type
      check_return self.class._set_numeric(@data, key, value, type)
    end
    
    def get_numeric key, type
      check_return self.class._get_numeric(@data, key, type)
    end
    
    def set_string key, value
      check_return self.class._set_string(@data, key, value)
    end
    
    def get_string key
      check_return self.class._get_string(@data, key)
    end
    
    def set_data key, value
      raise "Not Implemeneted: set_data()"
    
      check_return self.class._set_data(@data, key, value)
    end
    
    def get_data key
      raise "Not Implemented: get_data()"
      
      check_return self.class._get_data(@data, key)
    end    
    
    # Infered as int32_t or char*
    def []= k,v
      if v.is_a?(Fixnum)
        set_numeric k, v, NVS::INT32
        return v
      elsif v.is_a?(String)
        set_string k, v
        return v
      end
    end
    
    # inferred as int32_t
    def [] k
      get_numeric k, NVS::INT32
    end
    
    private
    def check_return res
      return res unless !res
    
      raise "TypeError: unsupported type" if res == nil
      raise "NVSError: (err != ESP_OK)"   if res == false
    end
  end
end           
