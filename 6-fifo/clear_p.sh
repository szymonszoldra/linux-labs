WE=we

if [ -d "$WE" ]; then
  rm -rf "$WE"
  mkdir "$WE"
  echo "Cleared $WE!"
fi