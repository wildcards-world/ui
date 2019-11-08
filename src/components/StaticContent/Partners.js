import React from "react";
import { Flex, Box, Card, Heading, Text } from "rimble-ui";
import { infoBackground } from "../../Styles.bs";
import UbisoftLogo from "../../img/logos/Ubisoft.png";
import EthCapeTownLogo from "../../img/logos/EthCapeTown.png";
import WildTomorrowFundLogo from "../../img/logos/WildTomorrowFund.png";

const styles = {
  logo: {
    marginLeft: "10%",
    width: "80%"
  },
  card: {
    height: "100%"
  }
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
            <img src={UbisoftLogo} style={styles.logo} />
            <Text>Ubisoft's Entrepreneurs Lab, Season 4, participants</Text>
          </a>
        </Card>
      </Box>
      <Box mt={70} mb={70} width={[1, 1, 0.2]} color="black">
        <Card style={styles.card}>
          <a href="https://ethcapetown.com/">
            <img src={EthCapeTownLogo} style={styles.logo} />
            <Text>Overall winners of EthCapeTown hackathon</Text>
          </a>
        </Card>
      </Box>
      <Box mt={70} mb={70} width={[1, 1, 0.2]} color="black">
        <Card style={styles.card}>
          <a href="https://wildtomorrowfund.org">
            <img src={WildTomorrowFundLogo} style={styles.logo} />
            <Text>Wild Tomorrow Fund, our wildlife conservation partner</Text>
          </a>
        </Card>
      </Box>
      <script
        data-who="💎 Made with naker.io 💎"
        src="https://harbor.naker.io/form/viewer.js"
        data-form='{"name":"whatever","environment":{"gradient":"horizontal","sensitivity":0.2,"colorStart":[216,250,183,0.75],"colorEnd":[87,233,70,0.62],"colorLight":[136,108,200,0.56]},"particle":{"url":"https://res.cloudinary.com/naker-io/image/upload/v1573156736/kdymaynm3c9zcicvbo8q.png","shape":"Pentagonal Prism","pieces":84,"size":35,"positionFusion":{"x":0,"y":0,"z":3.0635522446731356},"rotationFusion":{"x":0,"y":0,"z":0},"scalingFusion":{"x":0,"y":0,"z":0},"alphaFusion":false,"gravityExplosion":{"x":0,"y":0,"z":0},"powerExplosion":0.6}}'
      ></script>
    </Flex>
  </div>
);

export default Partners;
