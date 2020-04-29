import React from "react";
import { Card, Box, Flex } from "rimble-ui";

const styles = {
  background: {
    paddingTop: "1rem",
    backgroundImage: "linear-gradient(to top, #74C7D7 0%, white 100%)",
  },
  center: {
    textAlign: "center",
  },
};

export default ({ animalArray }) => {
  return (
    <div style={styles.background}>
      <div>
        <h1 style={{ paddingTop: "5rem", textAlign: "center" }}>
          Wildcards Kingdom
        </h1>
      </div>
      <Flex
        flexWrap={"wrap"}
        justifyContent={"space-around"}
        alignItems={"stretch"}
        px={50}
      >
        {animalArray.map((Animal, key) => (
          <Box key={key} fontSize={4} p={3} width={[1, 1, 0.3]}>
            <Card>
              <Animal />
            </Card>
          </Box>
        ))}
        {/* Always empty to ensure grid structure */}
        <Box fontSize={4} p={3} width={[1, 1, 0.3]}></Box>
        <Box fontSize={4} p={3} width={[1, 1, 0.3]}></Box>
      </Flex>
    </div>
  );
};
