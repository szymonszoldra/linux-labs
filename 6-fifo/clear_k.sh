WY=wy

if [ -d "$WY" ]; then
  rm -rf "$WY"
  mkdir "$WY"
  echo "Cleared $WY!"
fi

