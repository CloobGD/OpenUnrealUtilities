
# Open Unreal Utilities
The Open Unreal Utilties plugin is a general purpose utility plugin for Unreal Engine 4.

## Contents
So far the plugin contains mainly runtime utilities and some automated testing helpers. These utilities include:

- **Camera:** Blueprint utility functions for scene projections and render target handling
- **Flow Control:** Generic lock and request objects
- **Math:** Some math utility functions and blueprint exposure of FMath functionality
- **Templates:**
	- Container utilities (e.g. reverse iterator)
	- UE4 style adaptation of std::tie
	- Utility functions for interface pointer / TScriptInterface handling
	- Bitmask manipulation functions for native C++ and Blueprint compatible bitmask definitions
- **Traits:** Additional type traits
- **Automated Testing:** *see 'Automated Testing' section below*

For a more detailed overview of all utilities you should check out the plugin source code, as most of the documentation is provided in the form of source code comments.

## Automated Testing
A big majority of the utilities has unit tests that can be executed via the Session Frontend from within Unreal.
These tests are written with the help of testing utilities contained in the OpenUnrealUtilitiesTests module and may also be used for other plugins or game code.
The tests should also serve as examples how the targeted components are meant to be used.

## Licensing
Both Open Unreal Utilities [plugin](SampleProject/Plugins/OpenUnrealUtilities/LICENSE.md) and [sample project](LICENSE.md) are licensed under the MIT license.

## Contributing
You are invited to create pull-requests to the github source for any additions or modifications you make to the plugin:
https://github.com/JonasReich/OpenUnrealUtilities
