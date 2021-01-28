@decco.decode
type orgDescriptionArray = array<string>

module YoutubeVid = {
  @module("./StaticContent//YoutubeVideo.js") @react.component
  external make: (~videoCode: string) => React.element = "default"
}

module ComingSoonAnimal = {
  @react.component
  let make = (~image, ~onClick) =>
    <div
      className={
        open CssJs
        style(.[width(#percent(32.))])
      }>
      <img
        className={
          open CssJs
          style(.[width(#percent(100.)), Styles.imageHoverStyle])
        }
        onClick={_e => onClick()}
        src=image
      />
    </div>
}

module ImageCarousel = {
  @react.component
  let make = (
    ~orgComingSoon: array<
      QlHooks.LoadOrganisationData.LoadOrganisationData_inner.t_organisations_by_pk_unlaunched,
    >,
    ~selectedIndex,
  ) =>
    <ResponsiveCarousel
      showArrows=true
      showStatus=true
      showIndicators=true
      infiniteLoop=true
      showThumbs=false
      useKeyboardArrows=true
      autoPlay=true
      stopOnHover=true
      swipeable=true
      dynamicHeight=true
      emulateTouch=true
      selectedItem=selectedIndex>
      {React.array(
        orgComingSoon->Array.mapWithIndex((key, animal) =>
          animal.real_wc_photos[0]->Option.mapWithDefault(React.null, photos =>
            <img key={key->string_of_int} src={CONSTANTS.cdnBase ++ photos.image} />
          )
        ),
      )}
    </ResponsiveCarousel>
}

module ComingSoonModal = {
  @react.component
  let make = (
    ~selectedComingSoonAnimal,
    ~setSelectedComingSoonAnimal,
    ~orgComingSoon: array<
      QlHooks.LoadOrganisationData.LoadOrganisationData_inner.t_organisations_by_pk_unlaunched,
    >,
  ) => {
    let (openImage, setOpenImage) = React.useState(_ => None)

    <Rimble.Modal isOpen={selectedComingSoonAnimal->Option.isSome}>
      <Rimble.Card width=Rimble.AnyStr("80vw") p=0>
        <Rimble.Button.Text
          icononly=true
          icon="Close"
          color="moon-gray"
          position="absolute"
          right=0
          m=1
          top=0
          onClick={_ =>
            switch openImage {
            | Some(_) => setOpenImage(_ => None)
            | None => setSelectedComingSoonAnimal(None)
            }}
        />
        <Rimble.Box p=1 mb=1>
          {switch selectedComingSoonAnimal {
          | Some(selectedComingSoonAnimal) =>
            switch openImage {
            | Some(selectedIndex) => <ImageCarousel orgComingSoon selectedIndex />
            | None =>
              let animal = orgComingSoon->Array.getUnsafe(selectedComingSoonAnimal)

              <div>
                <h3
                  className={
                    open Css
                    style(list{textAlign(center)})
                  }>
                  {(animal.name->Option.getWithDefault("Unnamed") ++
                    animal.commonName->Option.mapWithDefault("", commonName => " - " ++ commonName))
                    ->React.string}
                </h3>
                <Rimble.Flex flexWrap="wrap" alignItems="start" alignContent="space-around">
                  <Rimble.Box
                    width=[1., 1., 0.49]
                    className={
                      open Css
                      style(list{textAlign(#center), alignSelf(center), padding(em(0.5))})
                    }>
                    <div
                      className={
                        open Css
                        style(list{maxHeight(#em(26.)), overflow(#scroll)})
                      }>
                      {React.array(
                        animal.description
                        ->QlHooks.animalDescription_decode
                        ->Result.getWithDefault([])
                        ->Array.mapWithIndex((i, paragraphText) =>
                          <p key={i->string_of_int}> {paragraphText->React.string} </p>
                        ),
                      )}
                    </div>
                  </Rimble.Box>
                  <Rimble.Box
                    width=[1., 1., 0.45]
                    className={
                      open Css
                      style(list{textAlign(#center), alignSelf(center), padding(em(2.))})
                    }>
                    <div
                      className={
                        open Css
                        style(list{maxHeight(#vh(80.))})
                      }>
                      <PhotoGallery
                        onClick={(_, photoData) => setOpenImage(_ => Some(photoData.index))}
                        targetRowHeight=30
                        photos={animal.real_wc_photos->Array.map(photo => {
                          open PhotoGallery
                          {
                            src: CONSTANTS.cdnBase ++ photo.image,
                            width: 4,
                            height: 3,
                          }
                        })}
                      />
                    </div>
                  </Rimble.Box>
                </Rimble.Flex>
              </div>
            }
          | None => React.null
          }}
        </Rimble.Box>
      </Rimble.Card>
    </Rimble.Modal>
  }
}

module OrgPage = {
  @react.component
  let make = (
    ~orgData: QlHooks.LoadOrganisationData.LoadOrganisationData_inner.t_organisations_by_pk,
    ~orgId,
    ~selectedWildcardKey: option<int>,
  ) => {
    let orgName = orgData.name
    let orgDescription = orgData.description->orgDescriptionArray_decode
    let orgAnimals = orgData.wildcard
    let orgComingSoon = orgData.unlaunched
    let selectedComingSoonAnimal =
      /* Find the index of the wildcard in the array and display that wildcard */
      selectedWildcardKey->Option.flatMap(wildcardKey =>
        orgComingSoon->Array.getIndexBy(orgAnimal => wildcardKey == orgAnimal.key)
      )
    let clearAndPush = RootProvider.useClearNonUrlStateAndPushRoute()
    let setSelectedComingSoonAnimal = optKey =>
      optKey
      ->Option.mapWithDefault(`#org/${orgId}`, key => `#org/${orgId}/${key->Int.toString}`)
      ->clearAndPush
    let orgAnimalsArray =
      orgAnimals->Array.map(animal => animal.id->Option.getWithDefault(TokenId.makeFromInt(99999)))
    let (totalCollectedMainnetEth, totalCollectMaticDai) = QlHooks.useTotalRaisedAnimalGroup(
      orgAnimalsArray,
    )

    let orgWebsite = orgData.website
    let optOrgYoutubeVid = orgData.youtube_vid
    let orgImage = Animal.useGetOrgImage(orgId)

    <div>
      <ComingSoonModal selectedComingSoonAnimal setSelectedComingSoonAnimal orgComingSoon />
      <div
        className={
          open Css
          style(list{width(#percent(100.))})
        }>
        <Rimble.Flex flexWrap="wrap" alignItems="start" alignContent="space-arround">
          <Rimble.Box
            width=[1., 1., 0.3333]
            className={
              open Css
              style(list{textAlign(#center), alignSelf(center), padding(em(2.))})
            }>
            <a
              className={Cn.fromList(list{
                Styles.navListText,
                {
                  open Css
                  style(list{fontSize(em(3.))})
                },
              })}
              target="_blank"
              rel="noopener noreferrer"
              href=orgWebsite>
              <img
                className={
                  open CssJs
                  style(.[
                    // borderRadius(`percent(100.)),
                    width(#vh(25.)),
                    height(#vh(25.)),
                    objectFit(#contain),
                    // objectFit(`cover),
                  ])
                }
                src=orgImage
              />
            </a>
            <br />
            <a
              className={Cn.fromList(list{
                Styles.navListText,
                {
                  open Css
                  style(list{fontSize(em(3.))})
                },
              })}
              target="_blank"
              rel="noopener noreferrer"
              href=orgWebsite>
              {orgName->React.string}
            </a>
            <br />
            <div
              className={
                open CssJs
                style(.[
                  maxHeight(#em(15.)),
                  overflowY(#auto),
                  overflowX(#hidden),
                  backgroundColor(rgb(240, 240, 240)),
                  padding(em(0.5)),
                  borderRadius(em(0.8)),
                ])
              }>
              {switch orgDescription {
              | Ok(descriptionArray) =>
                React.array(
                  descriptionArray->Array.mapWithIndex((i, paragraphText) =>
                    <p
                      key={i->string_of_int}
                      className={
                        open CssJs
                        style(.[textAlign(#justify)])
                      }>
                      {paragraphText->React.string}
                    </p>
                  ),
                )
              | Error(_) => <p> {"error loading description"->React.string} </p>
              }}
            </div>
            <br />
          </Rimble.Box>
          <Rimble.Box
            width=[1., 1., 0.3333]
            className={
              open Css
              style(list{alignSelf(center), padding(em(2.))})
            }>
            {switch optOrgYoutubeVid {
            | Some(videoCode) => <YoutubeVid videoCode />
            | None => React.null
            }}
            <h2> {"Total Raised"->React.string} </h2>
            {switch (totalCollectedMainnetEth, totalCollectMaticDai) {
            | (Some(mainnetEth), Some(maticDai)) => <Amounts.AmountRaised mainnetEth maticDai />
            | _ => "Loading"->React.string
            }}
          </Rimble.Box>
          <Rimble.Box
            width=[1., 1., 0.3333]
            className={
              open Css
              style(list{alignSelf(center), padding(em(2.))})
            }>
            {switch orgAnimals {
            | [] => <p> {"This organisation doesn't have any wildcards yet"->React.string} </p>
            | uniquePreviouslyOwnedTokens =>
              <React.Fragment>
                <Rimble.Heading> {"Organisations animals"->React.string} </Rimble.Heading>
                <Rimble.Flex flexWrap="wrap" className=UserProfile.centreAlignOnMobile>
                  {React.array(
                    uniquePreviouslyOwnedTokens->Array.map(animal => {
                      let tokenId = animal.id->Option.getWithDefault(TokenId.makeFromInt(99999))
                      <UserProfile.Token key={tokenId->TokenId.toString} tokenId />
                    }),
                  )}
                </Rimble.Flex>
              </React.Fragment>
            }}
            {switch orgComingSoon {
            | [] => React.null
            | _orgAnimals =>
              <React.Fragment>
                <Rimble.Heading> {"Coming soon"->React.string} </Rimble.Heading>
                <Rimble.Flex flexWrap="wrap" className=UserProfile.centreAlignOnMobile>
                  React.null
                  {React.array(
                    orgComingSoon->Array.mapWithIndex((key, animal) =>
                      animal.real_wc_photos[0]->Option.mapWithDefault(React.null, photos =>
                        <ComingSoonAnimal
                          key={key->string_of_int}
                          image={CONSTANTS.cdnBase ++ photos.image}
                          onClick={() => setSelectedComingSoonAnimal(Some(animal.key))}
                        />
                      )
                    ),
                  )}
                </Rimble.Flex>
              </React.Fragment>
            }}
          </Rimble.Box>
        </Rimble.Flex>
      </div>
    </div>
  }
}

@react.component
let make = (~orgId: string, ~selectedWildcardKey) => {
  let orgData = QlHooks.useLoadOrganisationQuery(orgId)

  <Rimble.Flex flexWrap="wrap" alignItems="center" className=Styles.topBody>
    {switch orgData {
    | Some(organisations_by_pk) =>
      let orgData = organisations_by_pk
      <OrgPage orgId orgData selectedWildcardKey />
    | None =>
      <div>
        <Rimble.Heading> {"Loading Organisation Profile"->React.string} </Rimble.Heading>
        <Rimble.Loader />
      </div>
    }}
  </Rimble.Flex>
}
