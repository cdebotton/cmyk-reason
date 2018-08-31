type cache;
[@bs.module "simple-cache-provider"] external createCache: unit => cache = "";

type resource;
[@bs.module "simple-cache-provider"]
external createResource: ('a => 'b, 'a => string) => resource = "";

[@bs.send] external read: (resource, cache, 'a) => 'b = "";

module Module = (Config: {module type t;}) => {};