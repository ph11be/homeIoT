<?php
/**
 * The base configuration for WordPress
 *
 * The wp-config.php creation script uses this file during the
 * installation. You don't have to use the web site, you can
 * copy this file to "wp-config.php" and fill in the values.
 *
 * This file contains the following configurations:
 *
 * * MySQL settings
 * * Secret keys
 * * Database table prefix
 * * ABSPATH
 *
 * @link https://codex.wordpress.org/Editing_wp-config.php
 *
 * @package WordPress
 */

// ** MySQL settings - You can get this info from your web host ** //
/** The name of the database for WordPress */
define('DB_NAME', 'wordpress');

/** MySQL database username */
define('DB_USER', 'root');

/** MySQL database password */
define('DB_PASSWORD', 'lfc.11');

/** MySQL hostname */
define('DB_HOST', 'localhost');

/** Database Charset to use in creating database tables. */
define('DB_CHARSET', 'utf8mb4');

/** The Database Collate type. Don't change this if in doubt. */
define('DB_COLLATE', '');

/**#@+
 * Authentication Unique Keys and Salts.
 *
 * Change these to different unique phrases!
 * You can generate these using the {@link https://api.wordpress.org/secret-key/1.1/salt/ WordPress.org secret-key service}
 * You can change these at any point in time to invalidate all existing cookies. This will force all users to have to log in again.
 *
 * @since 2.6.0
 */
define('AUTH_KEY',         'SRX/x{kSQSpe8^ppBHj`..d_i0?&@wEd;C&X{C<MhPl>P*UE`Ul*auF,Vnk4%gr[');
define('SECURE_AUTH_KEY',  'Ilm_yQ@jRQt_^o;u5LGKLnT/.$+h7(><l9ou+vPFu2|:i)sdesV+<]+./4/QFm+$');
define('LOGGED_IN_KEY',    '/J; |H(*{mupELN4pOO9@GQ>Rnc1nqgcr?tGpJkSgi}@;]-om%|uR/ ZM8)y{cFr');
define('NONCE_KEY',        '(ArCLm-S5SR?C|Y~Hq+behyh>J*[Pk1m;yL^l{.[-3iTx-s6AN6!QgDJ):0?!7y3');
define('AUTH_SALT',        'q^}z[_2Iy;u(]<Q&T3D=U_:<VN`^ZVjz F0d|:h^3WvlbWF/h.kpp8zSi~[t spr');
define('SECURE_AUTH_SALT', 'EP58l}{C=z9tk/cITyW8I.O}XEX=n%d.>Dh`A;0u@.k*j>z7|x*@83iAuU+2W*MU');
define('LOGGED_IN_SALT',   'EnR h&7285X8mR$[}kZW0r{m)!l@%V8#%fZWaA_!dd{qj;@|25{5|TN{rd$bz}r2');
define('NONCE_SALT',       'F#i>]~YABs|lX}Q)Ku9>>S/$ro&tg&2+E68cJ 5!7kA#qQ,H/Hc#2Ze?N_nWxj6)');

/**#@-*/

/**
 * WordPress Database Table prefix.
 *
 * You can have multiple installations in one database if you give each
 * a unique prefix. Only numbers, letters, and underscores please!
 */
$table_prefix  = 'wp_';

/**
 * For developers: WordPress debugging mode.
 *
 * Change this to true to enable the display of notices during development.
 * It is strongly recommended that plugin and theme developers use WP_DEBUG
 * in their development environments.
 *
 * For information on other constants that can be used for debugging,
 * visit the Codex.
 *
 * @link https://codex.wordpress.org/Debugging_in_WordPress
 */
define('WP_DEBUG', false);

/* That's all, stop editing! Happy blogging. */

/** Absolute path to the WordPress directory. */
if ( !defined('ABSPATH') )
	define('ABSPATH', dirname(__FILE__) . '/');

/** Sets up WordPress vars and included files. */
require_once(ABSPATH . 'wp-settings.php');
