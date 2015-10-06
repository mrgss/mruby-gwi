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
