{
  "include_dirs": [
    '<!(node -e "require(\'nan\')")'
  ],
  "includes": [
    '<!(node -e "console.log(require(\'path\').join(require(\'module\')._resolveLookupPaths(\'nbind\'), \'src\', \'nbind.gypi\'))")'
  ]
}