[@bs.module "rimble-ui"] [@bs.scope "Button"] [@react.component]
external make:
  (
    ~_as: string=?,
    ~className: string=?,
    ~href: string=?,
    ~target: string=?,
    ~children: React.element=?,
    ~onClick: ReactEvent.Form.t => unit=?,
    ~icononly: bool=?,
    ~icon: string=?,
    ~color: string,
    ~position: string,
    ~top: int,
    ~right: int,
    ~height: string=?,
    ~size: string=?,
    ~m: int
  ) =>
  React.element =
  "Text";

// Carousel, { Modal, ModalGateway
/*
 <ModalGateway>
   {viewerIsOpen
      ? <Modal onClose=closeLightbox>
          <Carousel
            currentIndex=currentImage
            views={photos.map(x => {...x, srcset: x.srcSet, caption: x.title})}
          />
        </Modal>
      : null}
 </ModalGateway>;
 */
