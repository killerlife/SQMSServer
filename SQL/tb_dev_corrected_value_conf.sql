/*
Navicat MySQL Data Transfer

Source Server         : 127.0.0.1
Source Server Version : 50087
Source Host           : localhost:3306
Source Database       : dev_monitor

Target Server Type    : MYSQL
Target Server Version : 50087
File Encoding         : 65001

Date: 2015-03-24 13:26:52
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for tb_dev_corrected_value_conf
-- ----------------------------
DROP TABLE IF EXISTS `tb_dev_corrected_value_conf`;
CREATE TABLE `tb_dev_corrected_value_conf` (
  `id` int(11) NOT NULL auto_increment,
  `cinema_name` varchar(200) default NULL,
  `cinema_hall` varchar(200) default NULL,
  `dev_ip` varchar(32) default NULL,
  `fix_ftl` float(15,5) default NULL,
  `fix_lc` float(15,5) default NULL COMMENT '声压左声道',
  `fix_rc` float(15,5) default NULL COMMENT '声压右声道',
  `fix_cc` float(15,5) default NULL COMMENT '声压中置',
  `fix_hb` float(15,5) default NULL COMMENT '声压重低音',
  `fix_la` float(15,5) default NULL COMMENT '声压左环绕音效',
  `fix_ra` float(15,5) default NULL COMMENT '声压右环绕音效',
  `start_datetime` time default NULL COMMENT '开始检测日期',
  `end_datetime` time default NULL COMMENT '结束简测时间',
  `script` varchar(400) default NULL,
  `sync_mode` int(3) default NULL,
  `port` int(7) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of tb_dev_corrected_value_conf
-- ----------------------------
INSERT INTO `tb_dev_corrected_value_conf` VALUES ('4', '金逸中关村', '1号厅', '192.168.100.101', '0.00000', '0.00000', '0.00000', '0.00000', '0.00000', '0.00000', '0.00000', '09:00:00', '11:30:00', null, '0', '10001');
INSERT INTO `tb_dev_corrected_value_conf` VALUES ('5', '金逸中关村', '2号厅', '192.168.100.102', '0.00000', '0.00000', '0.00000', '0.00000', '0.00000', '0.00000', '0.00000', '09:00:00', '11:30:00', null, '0', '10001');
INSERT INTO `tb_dev_corrected_value_conf` VALUES ('6', '金逸中关村', '3号厅', '192.168.100.103', '0.00000', '0.00000', '0.00000', '0.00000', '0.00000', '0.00000', '0.00000', '09:00:00', '11:30:00', null, '0', null);
INSERT INTO `tb_dev_corrected_value_conf` VALUES ('7', '金逸中关村', '4号厅', '192.168.100.104', '0.00000', '0.00000', '0.00000', '0.00000', '0.00000', '0.00000', '0.00000', '09:00:00', '11:30:00', null, '0', null);
INSERT INTO `tb_dev_corrected_value_conf` VALUES ('8', '金逸中关村', '5号厅', '192.168.100.105', '0.00000', '0.00000', '0.00000', '0.00000', '0.00000', '0.00000', '0.00000', '09:00:00', '11:30:00', null, '0', null);
INSERT INTO `tb_dev_corrected_value_conf` VALUES ('9', '金逸中关村', '6号厅', '192.168.100.106', '0.00000', '0.00000', '0.00000', '0.00000', '0.00000', '0.00000', '0.00000', '09:00:00', '11:30:00', null, '0', null);
INSERT INTO `tb_dev_corrected_value_conf` VALUES ('10', '金逸中关村', '7号厅', '192.168.100.107', '0.00000', '0.00000', '0.00000', '0.00000', '0.00000', '0.00000', '0.00000', '09:00:00', '11:30:00', null, '0', null);
