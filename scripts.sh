function buildtests() {
  files=""
  for d in `find chess/lib/GameEngine -type d`
  do
    files="$files $(buildcpp $d)"
  done
  for d in `find chess/test -type d`
  do
    files="$files $(buildcpp $d)"
  done
  main="lib/main.cpp"
  files=${files//$main/}
  g++ -std=c++17 $files -o tests
}

alias runtests='./tests'

function test() {
    buildtests
    runtests
}