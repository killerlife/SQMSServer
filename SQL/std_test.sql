/*
Navicat MySQL Data Transfer

Source Server         : localhost_3306
Source Server Version : 50611
Source Host           : localhost:3306
Source Database       : dev_monitor

Target Server Type    : MYSQL
Target Server Version : 50611
File Encoding         : 65001

Date: 2015-03-26 15:03:25
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for std_test
-- ----------------------------
DROP TABLE IF EXISTS `std_test`;
CREATE TABLE `std_test` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `lc` float(255,5) DEFAULT NULL,
  `rc` float(255,5) DEFAULT NULL,
  `cc` float(255,5) DEFAULT NULL,
  `hb` float(255,5) DEFAULT NULL,
  `la` float(255,5) DEFAULT NULL,
  `ra` float(255,5) DEFAULT NULL,
  `white_ftl` float(255,5) DEFAULT NULL,
  `red_ftl` float(255,5) DEFAULT NULL,
  `green_ftl` float(255,5) DEFAULT NULL,
  `blue_ftl` float(255,5) DEFAULT NULL,
  `temp` float(255,5) DEFAULT NULL,
  `cct` float(255,5) DEFAULT NULL,
  `cinema_hall` varchar(100) DEFAULT NULL,
  `rx` double(11,5) DEFAULT NULL,
  `ry` double(11,5) DEFAULT NULL,
  `gx` double(11,5) DEFAULT NULL,
  `gy` double(11,5) DEFAULT NULL,
  `bluex` double(11,5) DEFAULT NULL,
  `bluey` double(11,5) DEFAULT NULL,
  `wx` double(11,5) DEFAULT NULL,
  `wy` double(11,5) DEFAULT NULL,
  `lss` double(11,5) DEFAULT NULL,
  `rss` double(11,5) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of std_test
-- ----------------------------
INSERT INTO `std_test` VALUES ('1', '85.00000', '77.00000', '60.00000', '88.00000', '29.00000', '100.00000', '14.00000', '14.00000', '14.00000', '14.00000', '31.00000', '85.00000', '1号厅', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '80.00000', '80.00000');
INSERT INTO `std_test` VALUES ('2', '76.00000', '14.00000', '80.00000', '66.00000', '60.00000', '49.00000', '14.00000', '14.00000', '14.00000', '14.00000', '10.00000', '100.00000', '2号厅', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '80.00000', '80.00000');
INSERT INTO `std_test` VALUES ('3', '73.00000', '75.00000', '70.00000', '64.00000', '56.00000', '47.00000', '14.00000', '14.00000', '14.00000', '14.00000', '62.00000', '91.00000', '3号厅', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '80.00000', '80.00000');
INSERT INTO `std_test` VALUES ('4', '80.00000', '80.00000', '80.00000', '80.00000', '80.00000', '80.00000', '14.00000', '14.00000', '14.00000', '14.00000', '23.00000', '100.00000', '4号厅', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '80.00000', '80.00000');
INSERT INTO `std_test` VALUES ('5', '80.00000', '80.00000', '80.00000', '80.00000', '80.00000', '80.00000', '14.00000', '14.00000', '14.00000', '14.00000', '23.00000', '100.00000', '5号厅', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '80.00000', '80.00000');
INSERT INTO `std_test` VALUES ('6', '80.00000', '80.00000', '80.00000', '80.00000', '80.00000', '80.00000', '14.00000', '14.00000', '14.00000', '14.00000', '23.00000', '100.00000', '6号厅', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '80.00000', '80.00000');
INSERT INTO `std_test` VALUES ('7', '80.00000', '80.00000', '80.00000', '80.00000', '80.00000', '80.00000', '14.00000', '14.00000', '14.00000', '14.00000', '23.00000', '100.00000', '7号厅', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '0.30000', '80.00000', '80.00000');
