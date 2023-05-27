#!/bin/bash

# Prompt the user to input the new file name
read -p "Enter new Project: " new_project_name

# Set source and destination folder paths
source_folder="Examples/Linux/TP3D"
destination_folder="Projects/Linux/$new_project_name"

# Create the parent directory if it does not exist
mkdir -p "$destination_folder"

# Copy folder and all its contents
cp -R "$source_folder" "$destination_folder"

# mv "$destination_folder" "$"
