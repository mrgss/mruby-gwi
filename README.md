# mruby-gwi
A ruby minimalistic UI designed to work easily with mruby without
strange dependencies.

This gem is part of MRGSS gems, but it works perfectly without it.

## Compilation & Instalation
Just put it into your mrbgems directory, the gem handles the rest for you.

##Supported Platforms
Windows Only at the moment.

## Why not an existant GUI?
The indea under GWI is provide a really compact GUI to work on all devices as
an inspiration from material design, the gem is being worked as stand alone
c headers and code, so This may aim to be another project too.
Also, this is just a window, with some commodities like opening file dialogs,
color dialogs, etc. But the main UI aims to be a rendered material design like.

This gem is inspired by shoes in the way it aims to work.

## License
See LICENSE for more information, but in short terms it is APACHE 2.0 license.

## How do I make a program?

### Making aporgram is quite easy, just do:

```ruby
GWI.open("My window", 800, 600)
GWI.loop {
  # my loop code...
}
```

### Adding events works like jQuery:

```ruby
GWI.on("click") { |evt|
  puts "I clicked at (#{evt.x}, #{evt.y})
}
```

### Event List:

-TODO-
