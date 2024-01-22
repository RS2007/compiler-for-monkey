let hello =  fn(){
    let world = fn(){
      return "world";
    };
    return world();
};
puts(hello());
