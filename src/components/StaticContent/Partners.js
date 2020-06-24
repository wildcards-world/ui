import React from "react";
import { Flex, Box, Card, Text } from "rimble-ui";
import { infoBackground } from "../../Styles.bs";
const UbisoftLogo = "/img/logos/Ubisoft.png";
const EthCapeTownLogo = "/img/logos/EthCapeTown.png";
const CVLabsLogo = "/img/logos/cvlabszug.jpg";
const WildTomorrowFundLogo = "/img/logos/WildTomorrowFund.png";
const DarwinAnimalDoctorsLogo =
  "/img/conservation-partners/darwin-animal-doctors.svg";
const TheGreatWhaleConservancyLogo =
  "/img/conservation-partners/great-whale-conservancy.png";
const SendaVerdeLogo = "/img/logos/SendaVerde.png";
const BdiLogo = "/img/conservation-partners/bdi.svg";
const PangolinAficaLogo = "/img/conservation-partners/pangolin-africa.svg";
const careForWild =
  "https://dd2wadt5nc0o7.cloudfront.net/conservations/care-for-wild-non-profit.svg";
const lemurConservationNetwork =
  "https://dd2wadt5nc0o7.cloudfront.net/conservations/lemur-conservation-network.svg";
const sharkSpotters =
  "https://dd2wadt5nc0o7.cloudfront.net/conservations/shark-spotters.svg";

const styles = {
  logo: {
    marginLeft: "10%",
    width: "80%",
  },
  card: {
    height: "100%",
  },
  center: {
    textAlign: "center",
  },
};

const Partners = () => {
  const conservationPartners = [
    {
      name: "sendaverde",
      url: "http://www.sendaverde.org/",
      logo: SendaVerdeLogo,
    },
    {
      name: "wildtomorrowfund",
      url: "https://wildtomorrowfund.org",
      logo: WildTomorrowFundLogo,
    },
    {
      name: "darwinanimaldoctors",
      url: "http://darwinanimaldoctors.org/",
      logo: DarwinAnimalDoctorsLogo,
    },
    {
      name: "greatwhaleconservancy",
      url: "http://greatwhaleconservancy.org/",
      logo: TheGreatWhaleConservancyLogo,
    },
    {
      name: "Biodiversity and Development Institute",
      url: "http://thebdi.org/",
      logo: BdiLogo,
    },
    {
      name: "Pangolin Africa",
      url: "http://pangolin.africa/",
      logo: PangolinAficaLogo,
    },
    {
      name: "Lemur Conservation Network",
      url: "https://www.lemurconservationnetwork.org/",
      logo: lemurConservationNetwork,
    },
    {
      name: "Shark Spotters",
      url: "https://sharkspotters.org.za/",
      logo: sharkSpotters,
    },
    {
      name: "Care for Wild",
      url: "https://www.careforwild.co.za",
      logo: careForWild,
    },
  ];

  return (
    <div width="100%">
      <Flex
        flexWrap={"wrap"}
        justifyContent={"space-around"}
        alignItems={"stretch"}
        px={50}
        pt={50}
        style={{ backgroundColor: "#73C8D7" }}
      >
        <h1>Conservation Partners</h1>
      </Flex>
      <Flex
        flexWrap={"wrap"}
        justifyContent={"space-around"}
        alignItems={"stretch"}
        style={{ backgroundColor: "#73C8D7" }}
        px={50}
        pb={50}
      >
        {conservationPartners.map((conservation) => (
          <Box mt={20} mb={20} width={[0.45, 0.45, 0.18]} color="black">
            <Card style={styles.card}>
              <a href={conservation.url}>
                <img
                  src={conservation.logo}
                  alt={conservation.name}
                  style={styles.logo}
                />
              </a>
            </Card>
          </Box>
        ))}
        <Box width={[0.45, 0.45, 0.18]} color="black"></Box>
        <Box width={[0.45, 0.45, 0.18]} color="black"></Box>
        <Box width={[0.45, 0.45, 0.18]} color="black"></Box>
        <Box width={[0.45, 0.45, 0.18]} color="black"></Box>
      </Flex>
      <Flex
        flexWrap={"wrap"}
        justifyContent={"space-around"}
        alignItems={"stretch"}
        className={infoBackground}
        px={50}
      >
        <Box mt={70} mb={70} width={[1, 1, 0.2]} color="black">
          <Card style={styles.card}>
            <a href="https://www.ubisoft.com/en-us/company/start-ups/station-f.aspx">
              <img src={UbisoftLogo} alt="ubisoft" style={styles.logo} />
              <Text style={styles.center}>
                Ubisoft's Entrepreneurs Lab, Season 4, participants
              </Text>
            </a>
          </Card>
        </Box>
        <Box mt={70} mb={70} width={[1, 1, 0.2]} color="black">
          <Card style={styles.card}>
            <a href="https://ethcapetown.com/">
              <img
                src={EthCapeTownLogo}
                alt="eth-cape-town"
                style={styles.logo}
              />
              <Text style={styles.center}>
                Overall winners of EthCapeTown hackathon
              </Text>
            </a>
          </Card>
        </Box>
        <Box mt={70} mb={70} width={[1, 1, 0.2]} color="black">
          <Card style={styles.card}>
            <a href="https://cvvc.com/index.php">
              <img src={CVLabsLogo} alt="cv-labs" style={styles.logo} />
              <Text style={styles.center}>
                CV Labs Incubator Program, Batch 2
              </Text>
            </a>
          </Card>
        </Box>
      </Flex>
    </div>
  );
};

export default Partners;
