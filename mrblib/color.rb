#===============================================================================
# Copyright 2015 MRGSS developers                                              #
#                                                                              #
# Licensed under the Apache License, Version 2.0 (the "License");              #
# you may not use this file except in compliance with the License.             #
# You may obtain a copy of the License at                                      #
#                                                                              #
#    http://www.apache.org/licenses/LICENSE-2.0                                #
#                                                                              #
# Unless required by applicable law or agreed to in writing, software          #
# distributed under the License is distributed on an "AS IS" BASIS,            #
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.     #
# See the License for the specific language governing permissions and          #
# limitations under the License.                                               #
#===============================================================================
class GWI::Color

  attr_reader :red
  attr_reader :green
  attr_reader :blue
  attr_reader :alpha

  def initialize(red = 0, green = 0, blue = 0, alpha = 255)
    if red.is_a?(::GWI::Color)
      self.set(red.red, red.green, red.blue, red.alpha)
    else
      self.set(red, green, blue, alpha)
    end
  end

  def set(red, green, blue, alpha = nil)
    self.red = red
    self.green = green
    self.blue = blue
    self.alpha = alpha ? alpha : 0
  end

  def red=(value)
    unless value.is_a?(Numeric)
      raise TypeError.new("Value of red must be a Numeric.")
    end
    @red = value > 255 ? 255 : (value < 0 ? 0 : value.to_i)
  end

  def green=(value)
    unless value.is_a?(Numeric)
      raise TypeError.new("Value of green must be a Numeric.")
    end
    @green = value > 255 ? 255 : (value < 0 ? 0 : value.to_i)
  end

  def blue=(value)
    unless value.is_a?(Numeric)
      raise TypeError.new("Value of blue must be a Numeric.")
    end
    @blue = value > 255 ? 255 : (value < 0 ? 0 : value.to_i)
  end

  def alpha=(value)
    unless value.is_a?(Numeric)
      raise TypeError.new("Value of alpha must be a Numeric.")
    end
    @alpha = value > 255 ? 255 : (value < 0 ? 0 : value.to_i)
  end

end
