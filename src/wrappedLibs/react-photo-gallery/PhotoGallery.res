type photo = {
  @dead("photo.src") src: string,
  @dead("photo.width") width: int,
  @dead("photo.height") height: int,
}
type selectedPhoto = {
  @dead("selectedPhoto.photo") photo: photo,
  index: int,
  // next: photo,
  // previous: photo,
}

@module("react-photo-gallery") @react.component
external // onClick:
make: (
  ~photos: array<photo>,
  ~targetRowHeight: int,
  ~onClick: (ReactEvent.Mouse.t, selectedPhoto) => unit,
) => React.element = "default"
