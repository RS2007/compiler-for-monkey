let reduce = fn(arr,accum,function){
  let iter = fn(arr, result) {
  if (len(arr) == 0) {
    result;
  } else {
    iter(tail(arr), function(result, first(arr)));
  }};
  iter(arr,accum);
};

let sum = fn(arr){
  reduce(arr, 0, fn(initial, el) { initial + el });
};

sum([1,2,3,4]);
