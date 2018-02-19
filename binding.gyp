{
	'targets': [{
		"includes": [
			"auto.gypi"
		],
		'target_name': 'nbind',
		'sources': [
			'src/nbind.cc'
		],
		'include_dirs': [
			'./libs/link/include',
			'./libs/link/modules/asio-standalone/asio/include'
		],
		'conditions': [
			['OS=="mac"', {
				'defines': ['LINK_PLATFORM_MACOSX=1']
			}],
			['OS=="linux"', {
				'defines': ['LINK_PLATFORM_LINUX=1']
			}],
			['OS=="win"', {
				'defines': ['LINK_PLATFORM_WINDOWS=1', '_WIN32_WINNT=0x0501'],
				'defines!': ['_HAS_EXCEPTIONS=0'],
			}],
		],
	}],
	"includes": [
		"auto-top.gypi"
	]
}