import React from "react";
import { Flex, Box, Card, Text } from "rimble-ui";
import { infoBackground } from "../../Styles.bs";
import UbisoftLogo from "../../img/logos/Ubisoft.png";
import EthCapeTownLogo from "../../img/logos/EthCapeTown.png";
import CVLabsLogo from "../../img/logos/cvlabszug.jpg";
import WildTomorrowFundLogo from "../../img/logos/WildTomorrowFund.png";
import YourConservationLogo from "../../img/logos/YourConservation.png";
import SendaVerde from "../../img/logos/SendaVerde.png";

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
      <Box mt={70} mb={70} width={[1, 1, 0.26]} color="black">
        <Card style={styles.card}>
          <a href="https://www.ubisoft.com/en-us/company/start-ups/station-f.aspx">
            <img src={UbisoftLogo} alt="ubisoft" style={styles.logo} />
            <Text style={styles.center}>
              Ubisoft's Entrepreneurs Lab, Season 4, participants
            </Text>
          </a>
        </Card>
      </Box>
      <Box mt={70} mb={70} width={[1, 1, 0.26]} color="black">
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
      <Box mt={70} mb={70} width={[1, 1, 0.26]} color="black">
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
      mt={30}
    >
      <h1>Conservation Partners</h1>
    </Flex>
    <Flex
      flexWrap={"wrap"}
      justifyContent={"space-around"}
      alignItems={"stretch"}
      px={50}
    >
      <Box mt={30} mb={70} width={[1, 1, 0.26]} color="black">
        <Card style={styles.card}>
          <a href="http://www.sendaverde.org/">
            <img src={SendaVerde} alt="senda-verde" style={styles.logo} />
            <Text style={styles.center}>Senda Verde</Text>
          </a>
        </Card>
      </Box>
      <Box mt={30} mb={70} width={[1, 1, 0.26]} color="black">
        <Card style={styles.card}>
          <a href="https://wildtomorrowfund.org">
            <img
              src={WildTomorrowFundLogo}
              alt="wildtomorrowfund"
              style={styles.logo}
            />
            <Text style={styles.center}>Wild Tomorrow Fund</Text>
          </a>
        </Card>
      </Box>
      <Box mt={30} mb={70} width={[1, 1, 0.26]} color="black">
        <Card style={styles.card}>
          <a href="mailto:denham@wildcards.world">
            <img src={YourConservationLogo} alt="you" style={styles.logo} />
            <Text style={styles.center}>You?</Text>
          </a>
        </Card>
      </Box>
    </Flex>
  </div>
);

export default Partners;
