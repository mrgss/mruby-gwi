class GWI::Color
  
  attr_reader :red
  attr_reader :green
  attr_reader :blue
  attr_reader :alpha
  
  def initialize(red = 0, green = 0, blue = 0, alpha = 255)
    self.set(red, green, blue, alpha)
  end
  
  def set(red, green, blue, alpha = nil)
    self.red = red
    self.green = green
    self.blue = blue 
    self.alpha = alpha ? alpha : 0
  end
  
  def red=(value)
    @red = value > 255 ? 255 : (value < 0 ? 0 : value)
  end

  def green=(value)
    @green = value > 255 ? 255 : (value < 0 ? 0 : value)
  end
  
  def blue=(value)
    @blue = value > 255 ? 255 : (value < 0 ? 0 : value)
  end

  def alpha=(value)
    @alpha = value > 255 ? 255 : (value < 0 ? 0 : value)
  end  
  
end