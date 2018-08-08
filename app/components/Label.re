module LabelPoseConfig = {
  [@bs.deriving abstract]
  type state = {
    opacity: float,
    y: float,
  };

  [@bs.deriving abstract]
  type t = {
    visible: state,
    hidden: state,
    initialPose: string,
  };

  let config =
    t(
      ~visible=state(~opacity=1., ~y=-10.),
      ~hidden=state(~opacity=0., ~y=0.),
      ~initialPose="hidden",
    );

  let element = Pose.Label;

  type pose =
    | Visible
    | Hidden;

  let get =
    fun
    | Visible => "visible"
    | Hidden => "hidden";
};

module Label = Pose.Make(LabelPoseConfig);

let make = Label.make;