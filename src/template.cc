#include "template.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <functional>
#include <v8.h>
#include <boost/filesystem.hpp>

#include "utils.h"
#include "exceptions.h"

using namespace stork;

namespace fs = boost::filesystem;

const char* to_c_str(const v8::String::Utf8Value& value);
v8::Handle<v8::Value> emit_wrapper( const v8::Arguments& args );
void report_exception( v8::Isolate* isolate, v8::TryCatch* try_catch );

void escape( std::string& str ) {

    utils::replace( str, "\n", "\\n" );
    utils::replace( str, "\r", "\\r" );
    utils::replace( str, "\t", "\\t" );
    utils::replace( str, "\'", "\\'" );
}

FileTemplate FileTemplate::parse( const std::string& template_file ) {

    std::string contents = utils::read_file_contents( template_file );

    std::stringstream ss;

    size_t last = 0;
    size_t start = contents.find( "<%" );

    while( start != std::string::npos ) {

        std::string fragment = contents.substr( last, start - last );
        escape( fragment );

        ss << "emit( \'" << fragment << "\' );\n";

        bool direct = contents[start+2] == '=';
        start += direct? 3 : 2;

        size_t end = contents.find( "%>", start );

        fragment = contents.substr( start, end - start );

        if( direct ) {
            ss << "emit( " <<
                  utils::trim( fragment ) <<
                  " );\n";
        }else {
            ss << utils::trim( fragment ) << "\n";

            //trim a trailing \n if the line ended with a %>
            if( contents[end + 2] == '\n' ) {
                end += 3;
            }
        }
        last = end + 2;

        start = contents.find( "<%", end );
    }

    FileTemplate ret( template_file );
    ret.javascript_ = ss.str();

    std::cout << "Created new file template " << ret.path_ << std::endl;

    return ret;
}

void FileTemplate::instantiate(
                        const std::string& path,
                        const std::map<std::string, std::string>& params ) {

    (void)params;
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handle_scope(isolate);

    v8::Handle<v8::String> script_source = v8::String::New( 
                                                    this->javascript_.c_str()
                                                          );
    v8::Handle<v8::Value> script_name = v8::String::New( "Unnamed" );

    // Create a template for the global object.
    v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New();

    v8::Handle<v8::FunctionTemplate> fn = v8::FunctionTemplate::New(
                                            emit_wrapper,
                                            v8::External::New( this )
                                                                   );

    global->Set( v8::String::New("emit"), fn );

    // Create a new execution environment containing the built-in
    // functions
    v8::Handle<v8::Context> context = v8::Context::New(NULL, global);
    // Enter the newly created execution environment.
    v8::Context::Scope context_scope(context);

    bool report_exceptions = true;

    v8::Handle<v8::Script> script;
    {
        // Compile script in try/catch context.
        v8::TryCatch try_catch;
        script = v8::Script::Compile(script_source, script_name);
        if (script.IsEmpty()) {
            // Print errors that happened during compilation.
            if (report_exceptions) {
                report_exception( isolate, &try_catch );
            }
        }
    }

    {
        v8::TryCatch try_catch;

        script->Run();
        if ( try_catch.HasCaught() ) {
            if ( report_exceptions ) {
                report_exception(isolate, &try_catch);
            }
        }

        std::ofstream of;
        of.open( path );
        of << this->out_.str();
        of.close();
    }

}

v8::Handle<v8::Value> FileTemplate::emit_wrapper( const v8::Arguments& args ) {

    v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(
                                        args.Data()
                                     );

    void *ptr = field->Value();
    FileTemplate *tpl = static_cast<FileTemplate *>( ptr );

    return tpl->emit( args );
}

v8::Handle<v8::Value> FileTemplate::emit( const v8::Arguments& args ) {

    bool first = true;
    for( int i = 0; i < args.Length(); i++ ) {
        v8::HandleScope handle_scope( args.GetIsolate() );
        if( first ) {
            first = false;
        } else {
            this->out_ << " ";
        }
        v8::String::Utf8Value str(args[i]);

        this->out_ << to_c_str( str );
    }

    return v8::Undefined();
}

const char* to_c_str(const v8::String::Utf8Value& value) {
  return *value ? *value : "<string conversion failed>";
}

void report_exception( v8::Isolate* isolate, v8::TryCatch* try_catch ) {

    v8::HandleScope handle_scope( isolate );
    v8::String::Utf8Value exception( try_catch->Exception() );
    const char* exception_string = to_c_str( exception );
    v8::Handle<v8::Message> message = try_catch->Message();

    if( message.IsEmpty() ) {
        // V8 didn't provide any extra information about this error; just
        // print the exception.
        throw V8Exception( exception_string );
    } else {
        // Print (filename):(line number): (message).
        v8::String::Utf8Value filename(message->GetScriptResourceName());
        const char* filename_string = to_c_str(filename);
        int linenum = message->GetLineNumber();
        throw V8Exception( "%s:%i: %s\n",
                                filename_string,
                                linenum,
                                exception_string
                         );
    }
}



Template::Template( const std::string& path )
    : path_( path ) {

    if( !fs::exists( path ) ||
        !fs::is_directory( path ) ) {
        
        throw ArgumentException( "The template \"%s\", does not exist",
                                    path.c_str() );
    }

    utils::walk( path,
        [this]( const std::string& path ) {

        FileTemplate tpl = FileTemplate::parse( path );
        this->templates_.push_back( tpl );
    } );
}

void Template::instantiate(
        const std::string name,
        const std::map<std::string, std::string>& params ) {

    (void)params;

    fs::path project( name );
    fs::create_directory( project );

    for( FileTemplate& tpl : this->templates_ ) {

        std::string relative_path = tpl.path().substr(
                                                        this->path_.length(),
                                                        std::string::npos
                                                     );

        std::string new_path = name + relative_path;

        
        tpl.instantiate( new_path, params );
    }
}

