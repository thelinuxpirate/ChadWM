{ config, pkgs, lib, ... }:

let
  dmenu  = pkgs.callPackage ../default.nix { app = "dmenu"; nixpkgs = pkgs; };
in
{
  options.chadwm.enableDmenu = lib.mkOption {
    type = lib.types.bool;
    default = false;
    description = "Enables my custom Dmenu build";
  };

  config = {
    environment.systemPackages = lib.optionals config.sleepy.enableDmenu [
      dmenu
    ];
  };
}
