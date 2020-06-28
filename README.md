# VSLicenseCpp
VSLicenseCpp is a program that helps to get and set Visual Studio Community Edition trial expiration date. This project is for educational purposes only.

## Installation
Download the [latest version of VSLicenseCpp](https://github.com/BercX/VSLicenseCpp/releases/latest).

## Usage
1. Run Cmd or PowerShell as administrator.
2. Move to the `VSLicenseCpp.exe` folder.
3. Use the following commands.

Get expiration date
```cmd
VSLicenseCpp.exe --get --version=VS2019
```
Set expiration date.
```cmd
VSLicenseCpp.exe --set --version=VS2019 
```
```cmd
VSLicenseCpp.exe --set --version=VS2017 --add_days=14
```
If you don't specify `--add_days` option, 30 days will be added.

Available options for `--version`: `VS2015`, `VS2017`, `VS2019`


## Thanks
[Dmitrii](https://stackoverflow.com/users/10046552/dmitrii) for the answer to [this question](https://stackoverflow.com/questions/43390466/visual-studio-community-2017-is-a-30-day-trial/51570570#51570570).

[beatcracker](https://github.com/beatcracker) for  [VSCELicense](https://github.com/beatcracker/VSCELicense) PowerShell module which does the same thing.

[jarro2783](https://github.com/jarro2783) for [cxxopts](https://github.com/jarro2783/cxxopts) command line argument parser library.

## Contributing
Pull requests are welcome.

## License
[MIT](https://github.com/BercX/VSLicenseCpp/blob/master/LICENSE)
