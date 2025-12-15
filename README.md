# Neighborhood-Preserving Dimensionality Reduction for Multivariate Volume Rendering
Plugins for the [ManiVault](https://github.com/ManiVaultStudio/core) framework which let you visualize multivariate volumetric datasets and create user-friendly transfer functions based on dimensionality reduction techniques.

The plugins in this repository were written by Ravi Snellenberg for the Master thesis [Neighborhood-Preserving Dimensionality Reduction for Multivariate Volume Rendering](https://resolver.tudelft.nl/uuid:91d45452-416f-4fda-bfb5-261be169f958) (2025) and additionally published at the [Bio+MedVis Challenge @ IEEE VIS 2025](http://biovis.net/2025/biovisChallenges_vis/) as [t-SNE based Transfer Functions for Multi-attribute Volume Rendering](https://biovis.net/2025/data/BioMedVisChallenge2025submissions/3178/final.pdf) (2025). The original code is hosted at [github.com/Rsnelllenberg/VolumeProjectorPlugin](https://github.com/Rsnelllenberg/VolumeProjectorPlugin).

https://github.com/user-attachments/assets/a50c668d-30e0-4cd2-a6c8-6574041d27f9

## Basic instruction video
First, it explains the two ways to load in datasets, then it shows the basic ways to set up and use the transfer function and view widgets, and finally, it goes over some of the render modes

https://github.com/user-attachments/assets/ce592486-d5a1-4a5e-84f4-011fe753f0fb


## Build instructions
- This is a plugin for the [manivault studio framework](https://github.com/ManiVaultStudio). Go to the repository of the framework, and it has extensive explanation about how to build and run plugins for it. 
- It has an optional dependency for the [Faiss ANN library](github.com/facebookresearch/faiss)
- Before running the program, you might need to adjust some parameters in the DRVViewPlugin/volumeRenderer.h file:
  - _fullGPUMemorySize = Tells the program how much VRAM it can use (mainly important for the full data pipeline mode, apart from some warnings). The default is 2 GB
  - _hnswIndexFolder = Folder path where it saves/loads the constructed index to/from when using the hnsw ANN library, default: "C:/hnsw_index/"  (TODO make this a CMAKE parameter)
  - _hnswM, _hnswEfConstruction, _hwnsEfSearch = Parameters for the ANN algorithm library that is used for the full data pipeline rendermode
  - (only if you use FAISS) _nlist, _nprobe, _useFaissANN = Again, parameters for the FAISS ANN algorithms used in the full data pipeline rendermode
- You still need to add at least a dimensionality reduction plugin for this plugin to be usable, e.g., [t-SNE](https://github.com/ManiVaultStudio/t-SNE-Analysis) or [UMAP](https://github.com/ManiVaultStudio/UMAP-Plugin)
- Some example environments (datasets) can be found here: https://osf.io/bh37e/overview

## Code overview
The repository actually contains 4 separate plugins:
- DVRVolumeData: The VolumeData dataset type needed to be shared between all plugins
- DRVTransferFunction: is responsible for the transfer function widget, which is mostly responsible for creating the textures that are used by the DRVViewPlugin to describe the transfer function 
- DRVViewPlugin: Does the actual volumetric rendering and contains all the necessary shaders as well as any extra processing that possibly needs to happen
  - The most important files here are the volumeRenderer files, which actually handle most of the rendering logic.
- DRVVolumeLoader: This is a dataloader plugin, and its only job is to either take a binary file (with some extra information provided by the user) or a point cloud that has a separate spatial and value dataset (as is sometimes obtained from unpacking hf5d files). And convert them to the VolumeData dataset type that the plugins work with

All plugins have their UI elements in the Action folder, contained in their specific plugin folder.
And generally, the file ending in ...Plugin.cpp / ... Plugin.h handles the data linking and setup of the plugin, while the file ending in Widget handles most of the underlying data (unless it offloads some of its logic to other auxiliary classes specific to the plugin)
The res (resource) folder contains any textures and/or shaders

## Misc

This repository was originally built for a master's thesis project, and is as of today not fully polished yet:
- The code is also not completely bug-free, though it is generally in a usable state.
- Finally, there are still a lot of possible QOL changes that I did not come around to add
