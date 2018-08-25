[@bs.deriving abstract]
type rect =
  pri {
    bottom: float,
    height: float,
    left: float,
    right: float,
    top: float,
    width: float,
    x: int,
    y: int,
  };

[@bs.send] external getBoundingClientRect: Dom.element => rect = "";

module Window = {
  type t;
  [@bs.get] external innerHeight: t => float = "";
  [@bs.get] external innerWidth: t => float = "";
  [@bs.get] external pageYOffset: t => float = "";
  [@bs.get] external pageXOffset: t => float = "";
};

[@bs.val] external window: option(Window.t) = "";

module Document = {
  type t;
};

[@bs.val] external document: option(Document.t) = "";

module Body = {
  type t;
  [@bs.get] external offsetHeight: t => float = "";
  [@bs.get] external offsetWidth: t => float = "";
};

[@bs.get] external body: Document.t => Body.t = "";