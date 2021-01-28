WE=we
WY=wy

if [ -d "$WE" ]; then
  rm -rf "$WE"
  mkdir "$WE"
  echo "Cleared $WE!"
fi

if [ -d "$WY" ]; then
  rm -rf "$WY"
  mkdir "$WY"
  echo "Cleared $WY!"
fi

