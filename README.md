# mruby-string-ext-latin9 [![Build Status](https://travis-ci.com/appPlant/mruby-string-ext-latin9.svg?branch=master)](https://travis-ci.com/appPlant/mruby-string-ext-latin9) [![Build status](https://ci.appveyor.com/api/projects/status/0e2cc9xaxefuxyrv/branch/master?svg=true)](https://ci.appveyor.com/project/katzer/mruby-string-ext-latin9/branch/master)

Conversion from iso-8859-15 (Latin9) to UTF-8 for [mruby][mruby].

```ruby
puts "F\xf6rdertechnik".from_latin9
# => "Fördertechnik"

puts 'Fördertechnik'.to_latin9
# => "F\xf6rdertechnik"
```

__Note:__ The lib does not check if the string is already UTF-8 encoded. Converting a UTF-8 string from Latin-9 to UTF-8 results into a bad encoding result. You can use [mruby-string-is-utf8][mruby-string-is-utf8] for that.

## Installation

Add the line below to your `build_config.rb`:

```ruby
MRuby::Build.new do |conf|
  # ... (snip) ...
  conf.gem 'mruby-string-ext-latin9'
end
```

Or add this line to your aplication's `mrbgem.rake`:

```ruby
MRuby::Gem::Specification.new('your-mrbgem') do |spec|
  # ... (snip) ...
  spec.add_dependency 'mruby-string-ext-latin9'
end
```

## Development

Clone the repo:
    
    $ git clone https://github.com/appplant/mruby-string-ext-latin9 && cd mruby-string-ext-latin9/

Compile the source:

    $ rake compile

Run the tests:

    $ rake test

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/appplant/mruby-string-ext-latin9.

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request

## Authors

- Sebastián Katzer, Fa. appPlant GmbH

## License

The mgem is available as open source under the terms of the [MIT License][license].

Made with :yum: in Leipzig

© 2018 [appPlant GmbH][appplant]

[mruby]: https://github.com/mruby/mruby
[mruby-string-is-utf8]: https://github.com/Asmod4n/mruby-string-is-utf8
[license]: http://opensource.org/licenses/MIT
[appplant]: www.appplant.de
