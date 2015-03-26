/*
Navicat MySQL Data Transfer

Source Server         : 127.0.0.1
Source Server Version : 50087
Source Host           : localhost:3306
Source Database       : dev_monitor

Target Server Type    : MYSQL
Target Server Version : 50087
File Encoding         : 65001

Date: 2015-03-26 16:57:58
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for std_test
-- ----------------------------
DROP TABLE IF EXISTS `std_test`;
CREATE TABLE `std_test` (
  `id` int(11) NOT NULL auto_increment,
  `lc` float(255,5) default NULL,
  `rc` float(255,5) default NULL,
  `cc` float(255,5) default NULL,
  `hb` float(255,5) default NULL,
  `la` float(255,5) default NULL,
  `ra` float(255,5) default NULL,
  `white_ftl` float(255,5) default NULL,
  `red_ftl` float(255,5) default NULL,
  `green_ftl` float(255,5) default NULL,
  `blue_ftl` float(255,5) default NULL,
  `temp` float(255,5) default NULL,
  `cct` float(255,5) default NULL,
  `cinema_hall` varchar(100) default NULL,
  `rx` double(11,5) default NULL,
  `ry` double(11,5) default NULL,
  `gx` double(11,5) default NULL,
  `gy` double(11,5) default NULL,
  `bluex` double(11,5) default NULL,
  `bluey` double(11,5) default NULL,
  `wx` double(11,5) default NULL,
  `wy` double(11,5) default NULL,
  `lss` double(11,5) default NULL,
  `rss` double(11,5) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of std_test
-- ----------------------------
INSERT INTO `std_test` VALUES ('1', '85.00000', '85.00000', '85.00000', '85.00000', '85.00000', '85.00000', '14.00000', '2.95000', '10.11000', '0.97000', '31.00000', '85.00000', '1号厅', '0.68000', '0.32000', '0.26500', '0.69000', '0.15000', '0.06000', '0.31400', '0.35100', '85.00000', '85.00000');
INSERT INTO `std_test` VALUES ('2', '85.00000', '85.00000', '85.00000', '85.00000', '85.00000', '85.00000', '14.00000', '2.95000', '10.11000', '0.97000', '10.00000', '100.00000', '2号厅', '0.68000', '0.32000', '0.26500', '0.69000', '0.15000', '0.06000', '0.31400', '0.35100', '85.00000', '85.00000');
INSERT INTO `std_test` VALUES ('3', '85.00000', '85.00000', '85.00000', '85.00000', '85.00000', '85.00000', '14.00000', '2.95000', '10.11000', '0.97000', '62.00000', '91.00000', '3号厅', '0.68000', '0.32000', '0.26500', '0.69000', '0.15000', '0.06000', '0.31400', '0.35100', '85.00000', '85.00000');
INSERT INTO `std_test` VALUES ('4', '85.00000', '85.00000', '85.00000', '85.00000', '85.00000', '85.00000', '14.00000', '2.95000', '10.11000', '0.97000', '23.00000', '100.00000', '4号厅', '0.68000', '0.32000', '0.26500', '0.69000', '0.15000', '0.06000', '0.31400', '0.35100', '85.00000', '85.00000');
INSERT INTO `std_test` VALUES ('5', '85.00000', '85.00000', '85.00000', '85.00000', '85.00000', '85.00000', '14.00000', '2.95000', '10.11000', '0.97000', '23.00000', '100.00000', '5号厅', '0.68000', '0.32000', '0.26500', '0.69000', '0.15000', '0.06000', '0.31400', '0.35100', '85.00000', '85.00000');
INSERT INTO `std_test` VALUES ('6', '85.00000', '85.00000', '85.00000', '85.00000', '85.00000', '85.00000', '14.00000', '2.95000', '10.11000', '0.97000', '23.00000', '100.00000', '6号厅', '0.68000', '0.32000', '0.26500', '0.69000', '0.15000', '0.06000', '0.31400', '0.35100', '85.00000', '85.00000');
INSERT INTO `std_test` VALUES ('7', '85.00000', '85.00000', '85.00000', '85.00000', '85.00000', '85.00000', '14.00000', '2.95000', '10.11000', '0.97000', '23.00000', '100.00000', '7号厅', '0.68000', '0.32000', '0.26500', '0.69000', '0.15000', '0.06000', '0.31400', '0.35100', '85.00000', '85.00000');
