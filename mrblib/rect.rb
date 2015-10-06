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
class GWI::Rect

  attr_reader :x
  attr_reader :y
  attr_reader :width
  attr_reader :height

  def initialize(x = 0, y = 0, width = 0, height = 0)
    if x.is_a?(::GWI::Rect)
      self.set(x.x, x.y, x.width, x.height)
    else
      self.set(x, y, width, height)
    end
  end

  def set(x, y, width, height)
    self.x = x
    self.y = y
    self.width = width
    self.height = height
  end

  def x=(value)
    unless (value.is_a?(Numeric))
      raise TypeError.new("Value of x must be a Numeric.")
    end
    if (value < 0)
      raise ArgumentError.new("Value of x must be bigger than zero.")
    end
    @x = value.to_i
  end

  def y=(value)
    unless (value.is_a?(Numeric))
      raise TypeError.new("Value of y must be a Numeric.")
    end
    if (value < 0)
      raise ArgumentError.new("Value of y must be bigger than zero.")
    end
    @y = value.to_i
  end

  def width=(value)
    unless (value.is_a?(Numeric))
      raise TypeError.new("Value of y must be a Numeric.")
    end
    @width = value.to_i
  end

  def height=(value)
    unless (value.is_a?(Numeric))
      raise TypeError.new("Value of y must be a Numeric.")
    end
    @height = value.to_i
  end

end
