# Joint Market Model (JMM)

The Joint Market Model (JMM) is an energy system model that has been in use since 2005 in various European research groups as well as in industry. The model has its roots in the WILMAR project, an EU-funded project coordinated by RISOE National Laboratory in Denmark. The present report documents the version 2.1 of the JMM, which is based on Version 2.0. Version 2.0 has been almost entirely rewritten albeit keeping many of the features of the original model. Also the present documentation follows to some extent the documentation of the first model version , yet has been thoroughly revised and rewritten to describe the current version.
The JMM has a clear focus on market and system operation in the electricity market with high shares of renewable energies. Yet it also includes a detailed modelling of district heating and CHP units as well as demand-side flexibilities. Distinctive features of the JMM encompass a rolling planning approach which allows to model information arrival such as forecast updates and a detailed modelling of various markets, including an approach to model flow-based market coupling and other advanced formats of market coupling as currently in place in Central Western Europe.


## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development, testing and project purposes.

### Prerequisites

* GAMS
* MATLAB for automated runs,  LP2MIP automazation and CHP Tool
* MySQL databse 
* CHP Tool 

### Installing

1. Clone Repository into a "Project folder" (name can be choosen)
2. Rename Repository folder from `jmm` --> `model`
2. Run `install_JMM.bat` from the folder. This will create the necessary folder structure.

The folder structure has to follow this structure:
```
--"Project Folder"
  --model
    -- _default_Printout
    -- _default_Choice_file
    --...
  --Printout
    --"run folder"
      --inc_database
      --GDX_files
      --...	
```

## Contributing

### Branch structure

We use the branch structure suggested by @nive. You can find a detailed explanation [here](https://nvie.com/posts/a-successful-git-branching-model/).

Defined branches:
* `master` contains running versions of the JMM
  * `develop` is merged into `master` via release branch
  * should be used for projects without development
* `develop` is also executable and main development branch
  * `develop` should be used as starting point for developing projects
  * `feature_*` branches are merged into `develop`
* `feature_*` branches are for development of specific features, e.g. `feature_CCGT`
  * Usually ony one developer works on one `feature_*` branch
  * every feature has its own `feature_*` branch
  * `feature_*` branches are merged into develop with the help of a pull request

### Branch Rules

Use pull requests to contribute to `develop`.
Relese of new `master` version use pull requests, too.

* `master` 
  * 2 reviews are required
  * review from Code Owners are required (to be declared who is an codeowner)

* `develop`
  * 1 review is required



## Authors

* Christpoh Weber
* Michael Bucksteeg
* Björn Felten
* Thomas Kallabis
* Maren Kempgens
* Robin Leisen
* Jennifer Mikurda
* Caroline Podewski
* Julian Radek
* Marco S. Breder

[Chair for Management Science and Energy Economics, University of Duisburg Essen](https://www.ewl.wiwi.uni-due.de/)

## License

Should be added

## Acknowledgments

* ...

## Limited functionalities/open development

* Automated installation
* LP2MIP automazation


