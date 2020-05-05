function buildtests() {
  files=""
  for d in `find chess/lib/GameEngine -type d`
  do
    files="$files $(collectcpp $d)"
  done
  for d in `find chess/test -type d`
  do
    files="$files $(collectcpp $d)"
  done
  main="lib/main.cpp"
  files=${files//$main/}
  g++ -std=c++17 $files -o tests
}

function collectcpp() {
  files=""
  for c in $1/*.cpp; do
    files="$files $c"
  done
  echo $files
}

alias runtests='./tests'

function test() {
    buildtests
    runtests
}