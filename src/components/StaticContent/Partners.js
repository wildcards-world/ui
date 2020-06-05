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

const Partners = () => (
  <div width="100%">
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
    >
      <Box mt={30} mb={70} width={[1, 1, 0.3]} color="black">
        <Card style={styles.card}>
          <a href="http://www.sendaverde.org/">
            <img src={SendaVerdeLogo} alt="senda-verde" style={styles.logo} />
            {/* <Text style={styles.center}>Senda Verde</Text> */}
          </a>
        </Card>
      </Box>
      <Box mt={30} mb={70} width={[1, 1, 0.3]} color="black">
        <Card style={styles.card}>
          <a href="https://wildtomorrowfund.org">
            <img
              src={WildTomorrowFundLogo}
              alt="wildtomorrowfund"
              style={styles.logo}
            />
            {/* <Text style={styles.center}>Wild Tomorrow Fund</Text> */}
          </a>
        </Card>
      </Box>
      <Box mt={30} mb={70} width={[1, 1, 0.3]} color="black">
        <Card style={styles.card}>
          <a href="http://darwinanimaldoctors.org/">
            <img
              src={DarwinAnimalDoctorsLogo}
              alt="darwin animal doctors"
              style={styles.logo}
            />
            {/* <Text style={styles.center}>Darwin Animal Doctors</Text> */}
          </a>
        </Card>
      </Box>
      <Box mt={30} mb={70} width={[1, 1, 0.3]} color="black">
        <Card style={styles.card}>
          <a href="http://greatwhaleconservancy.org/">
            <img
              src={TheGreatWhaleConservancyLogo}
              alt="the great whale conservancy"
              style={styles.logo}
            />
            {/* <Text style={styles.center}>The Great Whale Conservancy</Text> */}
          </a>
        </Card>
      </Box>
      <Box mt={30} mb={70} width={[1, 1, 0.3]} color="black">
        <Card style={styles.card}>
          <a href="http://thebdi.org/">
            <img
              src={BdiLogo}
              alt="Biodiversity and Development Institute"
              style={styles.logo}
            />
            {/* <Text style={styles.center}>The Great Whale Conservancy</Text> */}
          </a>
        </Card>
      </Box>
      <Box mt={30} mb={70} width={[1, 1, 0.3]} color="black">
        <Card style={styles.card}>
          <a href="http://pangolin.africa/">
            <img
              src={PangolinAficaLogo}
              alt="Pangolin Africa"
              style={styles.logo}
            />
            {/* <Text style={styles.center}>The Great Whale Conservancy</Text> */}
          </a>
        </Card>
      </Box>
    </Flex>
  </div>
);

export default Partners;
